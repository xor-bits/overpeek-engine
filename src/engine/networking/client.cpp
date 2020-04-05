#include "client.hpp"

#include <enet/enet.h>

#include "engine/internal_libs.hpp"
#include "server.hpp"



namespace oe::networking {

	const uint8_t channel_count = 16;



	Client::Client() {
		m_peer = nullptr;
		m_address = new ENetAddress();
		m_keep_running = false;
		m_channel_id = 0;

		m_client = enet_host_create(
			NULL,
			4,
			channel_count,
			0,
			0
		);
		if (!m_client) {
			spdlog::critical("Could not create ENet client");
		}
	}

	Client::~Client() {
		close();
	}

	int Client::connect(std::string ip, int port, uint32_t timeout) {
		disconnect();

		enet_address_set_host(m_address, ip.c_str());
		m_address->port = port;

		mtx.lock();
		m_peer = enet_host_connect(m_client, m_address, channel_count, 0);
		mtx.unlock();
		if (!m_peer) {
			spdlog::critical("No available peers for initiating an ENet connection");
			return -1;
		}

		ENetEvent event;
		mtx.lock();
		if (enet_host_service(m_client, &event, timeout) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			enet_host_flush(m_client);
			if (m_callback_connect) m_callback_connect();
			m_connected = true;
		}
		else {
			enet_peer_reset(m_peer);
			spdlog::critical("failed to connect to {}:{}", ip, port);
			m_connected = false;
			mtx.unlock();
			return -1;
		}
		mtx.unlock();

		m_keep_running = true;
		m_thread = std::thread(&Client::operate, this);

		return 0;
	}

	int Client::disconnect() {
		if (!m_connected) {
			return -1;
		}

		// wait for event service to stop
		m_keep_running = false;
		if (m_thread.joinable()) m_thread.join();

		enet_peer_disconnect(m_peer, 0);
		ENetEvent event;
		while (true)
		{
			mtx.lock();
			if (enet_host_service(m_client, &event, 1000) <= 0) {
				mtx.unlock();
				break;
			}
			mtx.unlock();

			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				if (m_callback_disconnect) m_callback_disconnect();
				m_connected = false;
				return 0;
			}
		}

		enet_peer_reset(m_peer);
		if (m_callback_disconnect) m_callback_disconnect();
		m_connected = false;
		return 0;
	}

	int Client::close() {
		if (m_connected) {
			disconnect();
		}

		// close the server
		mtx.lock();
		if (m_client) enet_host_destroy(m_client);
		m_client = nullptr;
		mtx.unlock();
		return 0;
	}

	int Client::send(const unsigned char* bytes, size_t count) {
		m_channel_id++; if (m_channel_id >= channel_count) m_channel_id = 0;

		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_peer, m_channel_id, packet);
		mtx.lock();
		enet_host_flush(m_client);
		mtx.unlock();
		return 0;
	}

	void Client::operate() {
		ENetEvent event;

		while (m_keep_running) {
			mtx.lock();
			if (enet_host_service(m_client, &event, 0) <= 0) {
				mtx.unlock();
				continue;
			}
			mtx.unlock();

			if (event.type == ENET_EVENT_TYPE_NONE) {}
			else if (event.type == ENET_EVENT_TYPE_CONNECT) {
				if (m_callback_connect) m_callback_connect();
			}
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
				if (m_callback_disconnect) m_callback_disconnect();
			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				if (m_callback_recieve) m_callback_recieve(event.packet->data, event.packet->dataLength);
				enet_packet_destroy(event.packet);
			}
		}
	}

	float Client::getPacketLoss() {
		return static_cast<float>(m_peer->packetLoss) / static_cast<float>(ENET_PEER_PACKET_LOSS_SCALE);
	}

}