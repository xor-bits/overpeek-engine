#include "server.hpp"

#include <enet/enet.h>

#include "engine/internal_libs.hpp"



namespace oe::networking {

	const uint8_t channel_count = 16;

	bool enet::initialized = false;



	void enet::initEnet() {
		if (!initialized) {
			enet_initialize();
		}
	}
	
	void enet::deinitEnet() {
		if (initialized) {
			enet_deinitialize();
		}
	}

	Server::Server() {
		m_address = new ENetAddress();
		m_channel_id = 0;
	}

	Server::~Server() {
		close();
		delete m_address;
	}

	int Server::open(std::string ip, int port) {
		enet_address_set_host(m_address, ip.c_str());
		m_address->port = port;
		m_address->host = ENET_HOST_ANY;

		mtx.lock();
		m_server = enet_host_create(
			m_address,
			32,
			channel_count,
			0,
			0
		);

		if (!m_server) {
			spdlog::critical("Failed to create ENet server at port {}", port);
			m_opened = false;
			mtx.unlock();
			return -1;
		}
		mtx.unlock();
		m_opened = true;

		m_keep_running = true;
		m_thread = std::thread(&Server::operate, this);

		return 0;
	}
	
	int Server::close() {
		if (!m_opened) return -1;

		// wait for event service to stop
		m_keep_running = false;
		if (m_thread.joinable()) m_thread.join();

		// close the server
		mtx.lock();
		if (m_server) enet_host_destroy(m_server);
		m_server = nullptr;
		mtx.unlock();
		return 0;
	}

	int Server::send(const unsigned char* bytes, size_t count, size_t client_id) {
		m_channel_id++; if (m_channel_id >= channel_count) m_channel_id = 0;

		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_peers.at(client_id), m_channel_id, packet);
		mtx.lock();
		enet_host_flush(m_server);
		mtx.unlock();
		return 0;
	}

	int Server::send(const unsigned char* bytes, size_t count) {
		m_channel_id++; if (m_channel_id >= channel_count) m_channel_id = 0;
		
		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		mtx.lock();
		enet_host_broadcast(m_server, m_channel_id, packet);
		enet_host_flush(m_server);
		mtx.unlock();
		return 0;
	}

	void Server::operate() {
		ENetEvent event;

		

		while (m_keep_running) {
			mtx.lock();
			if (enet_host_service(m_server, &event, 0) <= 0) {
				mtx.unlock();
				continue;
			}
			mtx.unlock();

			if (event.type == ENET_EVENT_TYPE_NONE) {}
			else if (event.type == ENET_EVENT_TYPE_CONNECT) {
				size_t client_id = std::hash<std::string>{}(fmt::format("{}:{}", event.peer->address.host, event.peer->address.port));
				event.peer->data = (void*)client_id;

				m_peers.insert(std::make_pair((size_t)event.peer->data, event.peer));
				if (m_callback_connect) m_callback_connect((size_t)event.peer->data);
			}
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
				m_peers.erase((size_t)event.peer->data);
				if (m_callback_disconnect) m_callback_disconnect((size_t)event.peer->data);
			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				if (m_callback_recieve) m_callback_recieve((size_t)event.peer->data, event.packet->data, event.packet->dataLength);
				enet_packet_destroy(event.packet);
			}
		}
	}

	std::string Server::getClientIP(size_t client_id) {
		std::string ip;
		ip.resize(100);
		enet_address_get_host_ip(&m_peers.at(client_id)->address, ip.data(), 100);
		ip = std::string(ip.data());
		return ip;
	}

	int Server::getClientPort(size_t client_id) {
		return m_peers.at(client_id)->address.port;
	}

	float Server::getPacketLoss(size_t client_id) {
		return static_cast<float>(m_peers.at(client_id)->packetLoss) / static_cast<float>(ENET_PEER_PACKET_LOSS_SCALE);
	}

}