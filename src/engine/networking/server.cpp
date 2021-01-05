#include "server.hpp"
#include "enet_wrap.hpp"
#include "engine/internal_libs.hpp"



namespace oe::networking
{
	static bool initialized = false;
	void init_enet()
	{
		if(!initialized)
		{
			initialized = true;
			if(enet_initialize() < 0)
				spdlog::error("ENet init failed");
			spdlog::info("ENet initialized");
		}
	}



	struct server_enet_data
	{
		ENetHostWrapper m_server{};
		ENetAddress m_address{};
		ENetCompressor m_compressor{};
		std::unordered_map<size_t, ENetPeer*> m_peers;
		size_t m_channel_id = 0;
		size_t m_client_id_gen = 0;
	};



	Server::Server(size_t max_clients, size_t max_channels)
		: m_data(std::make_unique<server_enet_data>())
		, m_max_clients(max_clients)
		, m_max_channels(max_channels)
	{
		init_enet();
	}

	Server::~Server()
	{
		close();
	}

	result Server::open(uint16_t port)
	{
		// address
		m_data->m_address.port = port;
		m_data->m_address.host = ENET_HOST_ANY;
		
		// server
		std::scoped_lock lock(mtx);
		m_data->m_server = { &m_data->m_address, m_max_clients, m_max_channels };
		m_running = m_data->m_server.m_host != nullptr;
		if (!m_running)
			return result{ "Server could not be created" };

		enet_host_compress(m_data->m_server.m_host, &m_data->m_compressor);
		m_thread = std::thread(&Server::operate, this);

		return result{};
	}
	
	result Server::close()
	{
		if (!m_running)
			return result{ "Not running", false };

		// wait for event service to stop
		m_running = false;
		if (m_thread.joinable())
			m_thread.join();

		// close the server
		std::scoped_lock lock(mtx);
		m_data->m_server.destroy();
		return result{};
	}

	result Server::send_to(const unsigned char* bytes, size_t count, size_t client_id)
	{
		const size_t channel = next_channel();
		ENetPacket*	packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		
		// std::scoped_lock lock(mtx);
		enet_peer_send(m_data->m_peers.at(client_id), channel, packet);
		enet_host_flush(m_data->m_server.m_host);
		
		return result{};
	}

	result Server::send(const unsigned char* bytes, size_t count)
	{
		const size_t channel = next_channel();
		ENetPacket*	packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		
		// std::scoped_lock lock(mtx);
		enet_host_broadcast(m_data->m_server.m_host, channel, packet);
		enet_host_flush(m_data->m_server.m_host);

		return result{};
	}

	void Server::operate()
	{
		ENetEvent event{};
		size_t client_id;

		while (m_running)
		{
			enet_host_flush(m_data->m_server.m_host);
			const int32_t r = m_data->m_server.run_service(mtx, event, std::chrono::milliseconds(50));
			if (r < 0)
				spdlog::warn("Server ENet service error");
			if (r == 0)
				continue;

			switch (event.type)
			{
			default:/* case ENET_EVENT_TYPE_NONE: */
				break;
			
			case ENET_EVENT_TYPE_CONNECT:
				client_id = m_data->m_client_id_gen++;
				event.peer->data = reinterpret_cast<void*>(client_id);
				m_data->m_peers.insert(std::make_pair(client_id, event.peer));
				m_dispatcher.trigger(ServerConnectEvent{ client_id });
				break;
			
			case ENET_EVENT_TYPE_DISCONNECT:
				client_id = reinterpret_cast<size_t>(event.peer->data);
				m_data->m_peers.erase(client_id);
				m_dispatcher.trigger(ServerDisconnectEvent{ client_id });		
				break;
			
			case ENET_EVENT_TYPE_RECEIVE:
				client_id = reinterpret_cast<size_t>(event.peer->data);
				m_dispatcher.trigger(ServerReceiveEvent{ client_id, { event.packet->data, event.packet->dataLength } });
				enet_packet_destroy(event.packet);
				break;
			}
		}
	}
	
	[[nodiscard]] size_t Server::next_channel()
	{
		m_data->m_channel_id++; // overflow (not gonna happen) is just going to make the next step easy
		
		if (m_data->m_channel_id >= m_max_channels)
			m_data->m_channel_id = 0;
		
		return m_data->m_channel_id;
	}

	std::string Server::client_address(size_t client_id) const
	{
		std::string ip{ 100, '\0' };
		enet_address_get_host_ip(&m_data->m_peers.at(client_id)->address, ip.data(), 100);
		ip.erase(0, ip.find_first_not_of('\0'));
		return ip;
	}

	uint16_t Server::client_port(size_t client_id) const
	{
		return m_data->m_peers.at(client_id)->address.port;
	}

	float Server::client_packet_loss(size_t client_id) const
	{
		return static_cast<float>(m_data->m_peers.at(client_id)->packetLoss) / static_cast<float>(ENET_PEER_PACKET_LOSS_SCALE);
	}

}