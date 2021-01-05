#include "client.hpp"
#include "enet_wrap.hpp"
#include "engine/internal_libs.hpp"



namespace oe::networking
{
	struct client_enet_data
	{
		ENetHostWrapper m_client{};
		ENetAddress m_address{};
		ENetCompressor m_compressor{};
		ENetPeer* m_peer = nullptr;
		size_t m_channel_id = 0;
	};



	Client::Client(size_t max_channels)
		: m_data(std::make_unique<client_enet_data>())
		, m_max_channels(max_channels)
	{
		init_enet();
	}

	Client::~Client()
	{
		close();
	}

	result Client::connect(const std::string& ip, uint16_t port, std::chrono::milliseconds timeout)
	{
		{
			std::scoped_lock lock(mtx);
			if(!m_data->m_client.m_host)
			{
				m_data->m_client = { nullptr, 1, 1 };
				if(!m_data->m_client.m_host)
					return result{ "Client could not be created" };

				enet_host_compress(m_data->m_client.m_host, &m_data->m_compressor);
			}
		}

		// address
		enet_address_set_host(&m_data->m_address, ip.c_str());
		m_data->m_address.port = port;

		// disconnect and then connect
		disconnect();
		{
			std::scoped_lock lock(mtx);
			m_data->m_peer = enet_host_connect(m_data->m_client.m_host, &m_data->m_address, m_max_channels, 0);
			if (m_data->m_peer == nullptr)
				return result{ "No available peers for initiating an ENet connection" };
		}
		

		// operate until it fails to connect or connects successfully
		ENetEvent event;
		const int32_t service_return = m_data->m_client.run_service(mtx, event, timeout);
		m_running = service_return > 0 && event.type == ENET_EVENT_TYPE_CONNECT;
		
		if(!m_running)
		{
			std::scoped_lock lock(mtx);
			enet_peer_reset(m_data->m_peer);

			if(service_return == 0)
				return result{ "Timeout" };
			else
				return result{ "Connection failed" };
		}

		m_thread = std::thread(&Client::operate, this);
		return result{};
	}

	result Client::disconnect()
	{
		if (!m_running)
			return result{ "Not connected", false };

		// wait for event service to stop
		m_running = false;
		if (m_thread.joinable())
			m_thread.join();

		// send the disconnect request
		{
			std::scoped_lock lock(mtx);
			enet_peer_disconnect(m_data->m_peer, 0);
		}

		ENetEvent event;
		while (true)
		{
			const int32_t service_return = m_data->m_client.run_service(mtx, event, std::chrono::seconds{ 1 });
			if (service_return <= 0)
				break; // error or timeout

			switch (event.type)
			{
			default:/* case ENET_EVENT_TYPE_NONE: */
				break;
			
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			
			case ENET_EVENT_TYPE_DISCONNECT:
				return result{};
			}
		}

		disconnect_force();
		return result{ "Disconnect forced", false };
	}
	
	result Client::disconnect_force()
	{
		std::scoped_lock lock(mtx);
		enet_peer_reset(m_data->m_peer);
		return result{};
	}

	result Client::close()
	{
		disconnect();

		// close the server
		std::scoped_lock lock(mtx);
		m_data->m_client.destroy();
		return result{};
	}

	result Client::send(const uint8_t* bytes, size_t count)
	{
		const size_t channel = next_channel();
		ENetPacket*	packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		
		// std::scoped_lock lock(mtx);
		enet_peer_send(m_data->m_peer, channel, packet);
		enet_host_flush(m_data->m_client.m_host);

		return result{};
	}

	void Client::operate()
	{
		ENetEvent event{};

		while (m_running)
		{
			enet_host_flush(m_data->m_client.m_host);
			const int32_t r = m_data->m_client.run_service(mtx, event, std::chrono::milliseconds(50));
			if (r < 0)
				spdlog::warn("Client ENet service error");
			if (r == 0)
				continue;

			switch (event.type)
			{
			default:/* case ENET_EVENT_TYPE_NONE: */
				break;
			
			case ENET_EVENT_TYPE_DISCONNECT:
				m_dispatcher.trigger(ClientDisconnectEvent{});
				break;
			
			case ENET_EVENT_TYPE_RECEIVE:
				m_dispatcher.trigger(ClientReceiveEvent{ { event.packet->data, event.packet->dataLength } });
				enet_packet_destroy(event.packet);
				break;
			}
		}
	}
	
	[[nodiscard]] size_t Client::next_channel()
	{
		m_data->m_channel_id++; // overflow (not gonna happen) is just going to make the next step easy
		
		if (m_data->m_channel_id >= m_max_channels)
			m_data->m_channel_id = 0;
		
		return m_data->m_channel_id;
	}

	[[nodiscard]] std::string Client::server_address() const
	{
		std::string ip{ 100, '\0' };
		enet_address_get_host_ip(&m_data->m_address, ip.data(), 100);
		ip.erase(0, ip.find_first_not_of('\0'));
		return ip;
	}

	[[nodiscard]] uint16_t Client::server_port() const
	{
		return m_data->m_address.port;
	}

	[[nodiscard]] float Client::server_packet_loss() const
	{
		return static_cast<float>(m_data->m_peer->packetLoss) / static_cast<float>(ENET_PEER_PACKET_LOSS_SCALE);
	}

}