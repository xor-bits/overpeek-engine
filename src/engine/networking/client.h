#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <functional>



struct _ENetPeer; struct _ENetAddress; struct _ENetEvent; struct _ENetHost;
namespace oe::networking {

	class Client {
	private:
		_ENetPeer* m_peer = nullptr;
		_ENetAddress* m_address = nullptr;
		_ENetEvent* m_event = nullptr;
		_ENetHost* m_client = nullptr;
		std::atomic<bool> m_keep_running = false;
		std::thread m_thread;

		typedef std::function<void()> func_connect;
		typedef func_connect func_disconnect;
		typedef std::function<void(const unsigned char* data, size_t size)> func_recieve;

		func_connect m_callback_connect = nullptr;
		func_disconnect m_callback_disconnect = nullptr;
		func_recieve m_callback_recieve = nullptr;

		bool m_connected = false;

		void operate();

	public:
		Client();
		~Client();

		// timeout in milliseconds
		int connect(std::string ip, int port, uint32_t timeout = 3000);
		int disconnect();
		int close();
		int send(const unsigned char* bytes, size_t count);

		void setConnectCallback(func_connect callback_connect) { m_callback_connect = callback_connect; }
		void setDisconnectCallback(func_disconnect callback_disconnect) { m_callback_disconnect = callback_disconnect; }
		void setReciveCallback(func_recieve callback_recieve) { m_callback_recieve = callback_recieve; }

	};

}