#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <unordered_map>
#include <mutex>



struct _ENetHost; struct _ENetAddress; struct _ENetPeer; struct _ENetCompressor;
namespace oe::networking {

	class enet {
	private:
		static bool initialized;

	public:
		static void initEnet();
		static void deinitEnet();
	};

	class Server {
	private:
		_ENetAddress* m_address = nullptr;
		_ENetHost* m_server = nullptr;
		_ENetCompressor* m_compressor = nullptr;

		std::mutex mtx;

		std::atomic<bool> m_keep_running = false;
		std::thread m_thread;
		std::unordered_map<size_t, _ENetPeer*> m_peers = std::unordered_map<size_t, _ENetPeer*>();

		uint8_t m_channel_id;

		typedef std::function<void(size_t client_id)> func_connect;
		typedef func_connect func_disconnect;
		typedef std::function<void(size_t client_id, const uint8_t* data, size_t size)> func_recieve;

		func_connect m_callback_connect = nullptr;
		func_disconnect m_callback_disconnect = nullptr;
		func_recieve m_callback_recieve = nullptr;

		bool m_opened = false;

		void operate();

	public:
		Server();
		~Server();

		int open(std::string ip, int port);
		int close();
		int send(const uint8_t* bytes, size_t count, size_t client_id); // send to specific client
		int send(const uint8_t* bytes, size_t count); // send to all clients

		void setConnectCallback(func_connect callback_connect) { m_callback_connect = callback_connect; }
		void setDisconnectCallback(func_disconnect callback_disconnect) { m_callback_disconnect = callback_disconnect; }
		void setReciveCallback(func_recieve callback_recieve) { m_callback_recieve = callback_recieve; }

		std::string getClientIP(size_t client_id);
		int getClientPort(size_t client_id);
		float getPacketLoss(size_t client_id);

	};

}