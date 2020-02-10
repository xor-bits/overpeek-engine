#include "client.h"

#include <enet/enet.h>

#include "engine/internal_libs.h"
#include "server.h"



namespace oe::networking {

	Client::Client() {
		m_peer = nullptr;
		m_event = new ENetEvent();
		m_address = new ENetAddress();
		m_keep_running = false;

		m_client = enet_host_create(
			NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			2 /* allow up 2 channels to be used, 0 and 1 */,
			0 /* assume any amount of incoming bandwidth */,
			0 /* assume any amount of outgoing bandwidth */
		);
		if (!m_client) {
			spdlog::error("Could not create ENet client");
		}
	}

	Client::~Client() {
		close();
	}

	int Client::connect(std::string ip, int port, uint32_t timeout) {
		enet_address_set_host(m_address, ip.c_str());
		m_address->port = port;

		m_peer = enet_host_connect(m_client, m_address, 2, 0);
		if (!m_peer) {
			spdlog::critical("No available peers for initiating an ENet connection");
			return -1;
		}

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service(m_client, m_event, timeout) > 0 && m_event->type == ENET_EVENT_TYPE_CONNECT) {
			enet_host_flush(m_client);
			spdlog::info("connected to {}:{}", ip, port);
			m_connected = true;
		}
		else {
			enet_peer_reset(m_peer);
			spdlog::critical("failed to connect to {}:{}", ip, port);
			m_connected = false;
			return -1;
		}

		m_keep_running = true;
		m_thread = std::thread(&Client::operate, this);

		return 0;
	}

	int Client::disconnect() {
		if (!m_connected) return -1;

		enet_peer_disconnect(m_peer, 0);
		while (enet_host_service(m_client, m_event, 1000) > 0)
		{
			switch (m_event->type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(m_event->packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				return 0;
			}
		}

		enet_peer_reset(m_peer);
		return 0;
	}

	int Client::close() {
		if (!m_connected) return -1;

		// wait for event service to stop
		m_keep_running = false;
		if (m_thread.joinable()) m_thread.join();

		// close the server
		if (m_client) enet_host_destroy(m_client);
		m_client = nullptr;
		return 0;
	}

	int Client::send(const unsigned char* bytes, size_t count) {
		ENetPacket* packet = enet_packet_create(bytes, count, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_peer, 0, packet);
		enet_host_flush(m_client);
		return 0;
	}

	void Client::operate() {
		ENetEvent event;

		while (m_keep_running && enet_host_service(m_client, &event, 500) >= 0) {

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

			if (!m_keep_running) {
				break;
			}
		}
	}

}