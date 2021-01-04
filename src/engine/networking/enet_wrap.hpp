#pragma once

#include <enet/enet.h>



namespace oe::networking
{
	struct ENetHostWrapper
	{
		ENetHost* m_host = nullptr;

		ENetHostWrapper() = default;

		ENetHostWrapper(const ENetAddress * addr, size_t peer_count, size_t channels, enet_uint32 in_band = 0, enet_uint32 out_band = 0)
		{
			m_host = enet_host_create(addr, peer_count, channels, in_band, out_band);
		}

		ENetHostWrapper(ENetHostWrapper&& move)
		{
			m_host = move.m_host;
			move.m_host = nullptr;
		}

		ENetHostWrapper(const ENetHostWrapper& copy) = delete;

		~ENetHostWrapper()
		{
			destroy();
		}

		ENetHostWrapper& operator=(ENetHostWrapper&& move)
		{
			m_host = move.m_host;
			move.m_host = nullptr;
			return *this;
		}

		ENetHostWrapper& operator=(const ENetHostWrapper& copy) = delete;

		inline void destroy()
		{
			if(m_host)
				enet_host_destroy(m_host);
			m_host = nullptr;
		}

		// timeout 0 for no timeout
		inline int32_t run_service(std::mutex& mtx, ENetEvent& event, std::chrono::milliseconds timeout = std::chrono::seconds{ 10 })
		{
			if(!m_host)
				return -55555;

			std::scoped_lock lock(mtx);
			return enet_host_service(m_host, &event, timeout.count());
		};
	};
}