#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <mutex>

#include "shared.hpp"



namespace oe::networking
{
	struct client_enet_data;

	class Client
	{
	private:
		std::unique_ptr<client_enet_data> m_data;

		std::mutex mtx;
		std::atomic<bool> m_running = false;
		std::thread m_thread;
		size_t m_max_channels;

		void operate();
		[[nodiscard]] size_t next_channel();

	public:
		entt::dispatcher m_dispatcher;
		
		Client(size_t max_channels = 32);
		~Client();

		// timeout 0 for no timeout
		result connect(const std::string& ip, uint16_t port, std::chrono::milliseconds timeout = std::chrono::seconds{ 10 });
		result disconnect();
		result disconnect_force();
		result close();
		result send(const uint8_t* bytes, size_t count);

		/* contiguous_iterator_tag */
		template<typename Iterator>
		inline result send(Iterator begin, Iterator end)
		{
			if (begin == end) return result{};
			return send(reinterpret_cast<const uint8_t*>(&*begin), std::distance(begin, end) * sizeof(typename std::iterator_traits<Iterator>::value_type));
		}

		[[nodiscard]] std::string server_address() const;
		[[nodiscard]] uint16_t server_port() const;
		[[nodiscard]] float server_packet_loss() const;

	};

}