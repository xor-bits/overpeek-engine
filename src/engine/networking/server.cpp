#include "server.h"

#include <enet/enet.h>

#include "engine/internal_libs.h"



namespace oe::networking {

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
	}

	Server::~Server() {
		close();
		delete m_address;
	}

	int Server::open(std::string ip, int port) {
		enet_address_set_host(m_address, ip.c_str());
		m_address->port = port;
		m_address->host = ENET_HOST_ANY;

		m_server = enet_host_create(
			m_address /* the address to bind the server host to */,
			32        /* allow up to 32 clients and/or outgoing connections */,
			2		  /* allow up to 2 channels to be used, 0 and 1 */,
			0		  /* assume any amount of incoming bandwidth */,
			0		  /* assume any amount of outgoing bandwidth */
		);

		if (!m_server) {
			spdlog::critical("Failed to create ENet server at port {}", port);
			m_opened = false;
			return -1;
		}
		spdlog::info("ENet server opened at port {}", port);
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
		if (m_server) enet_host_destroy(m_server);
		m_server = nullptr;
		return 0;
	}

	int Server::send(const unsigned char* bytes, size_t count, size_t client_id) {
		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);

		enet_peer_send(m_peers.at(client_id), 0, packet);
		enet_host_flush(m_server);

		return 0;
	}

	int Server::send(const unsigned char* bytes, size_t count) {
		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		
		enet_host_broadcast(m_server, 0, packet);
		enet_host_flush(m_server);
		
		return 0;
	}

	void Server::operate() {
		ENetEvent event;

		while (m_keep_running && enet_host_service(m_server, &event, 500) >= 0) {

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

			if (!m_keep_running) {
				break;
			}
		}
	}

}