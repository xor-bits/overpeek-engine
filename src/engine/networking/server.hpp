#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <entt/entt.hpp>

#include "shared.hpp"



namespace oe::networking
{
	void init_enet();
	struct server_enet_data;

	
	
	class Server
	{
	private:
		std::unique_ptr<server_enet_data> m_data;

		std::mutex mtx;
		std::atomic<bool> m_running = false;
		std::thread m_thread;
		size_t m_max_clients;
		size_t m_max_channels;

		void operate();
		[[nodiscard]] size_t next_channel();

	public:
		entt::dispatcher m_dispatcher;
		
		Server(size_t max_clients = 32, size_t max_channels = 32);
		~Server();

		result open(uint16_t port);
		result close();
		result send_to(const uint8_t* bytes, size_t count, size_t client_id); // send to specific client
		result send(const uint8_t* bytes, size_t count); // send to all clients
		[[nodiscard]] inline bool running() const noexcept { return m_running; }
		
		/* contiguous_iterator_tag */
		template<typename Iterator>
		inline result send_to(Iterator begin, Iterator end, size_t client_id)
		{
			if (begin == end) return result{};
			return send_to(reinterpret_cast<const uint8_t*>(&*begin), std::distance(begin, end) * sizeof(typename std::iterator_traits<Iterator>::value_type), client_id);
		}
		/* contiguous_iterator_tag */
		template<typename Iterator>
		inline result send(Iterator begin, Iterator end)
		{
			if (begin == end) return result{};
			return send(reinterpret_cast<const uint8_t*>(&*begin), std::distance(begin, end) * sizeof(typename std::iterator_traits<Iterator>::value_type));
		}

		[[nodiscard]] std::string client_address(size_t client_id) const;
		[[nodiscard]] uint16_t client_port(size_t client_id) const;
		[[nodiscard]] float client_packet_loss(size_t client_id) const;

	};

}