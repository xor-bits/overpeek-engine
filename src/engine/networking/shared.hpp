#pragma once

#include <gsl/span>
#include "engine/engine.hpp"



namespace oe::networking
{
	void init_enet();
	
	
	
	class result
	{
	private:
		std::string m_message;
		bool m_failed;

	public:
		inline result()
			: m_message("Success"), m_failed(false)
		{}

		inline result(const std::string& message, bool failed = true)
			: m_message(message), m_failed(failed)
		{}

		inline bool failed() const { return m_failed; }
		inline const std::string& message() const { return m_message; }
	};

	

	struct ServerConnectEvent
	{
		size_t client_id = 0;
	};

	struct ServerDisconnectEvent
	{
		size_t client_id = 0;
	};

	struct ServerReceiveEvent
	{
		size_t client_id = 0;
		gsl::span<uint8_t> data;
	};

	struct ClientDisconnectEvent
	{};

	struct ClientReceiveEvent
	{
		gsl::span<uint8_t> data;
	};
}