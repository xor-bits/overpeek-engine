#pragma once

#include <entt/signal/dispatcher.hpp>
#include <entt/signal/delegate.hpp>
#include <tuple>
#include <functional>



namespace oe::graphics { class IWindow; class Window; }
namespace oe::utils { class GameLoop; }
namespace entt { class dispatcher; }

namespace oe::utils
{
	struct connect_guard_data_t { virtual ~connect_guard_data_t() {}; };

	class connect_guard
	{
	private:
		std::function<void()> m_connector_fn;
		std::function<void()> m_disconnector_fn;
		std::unique_ptr<connect_guard_data_t> data;
	
	public:
		connect_guard() = default;

		connect_guard& operator= (const connect_guard& copy)
		{
			m_connector_fn = copy.m_connector_fn;
			m_disconnector_fn = copy.m_disconnector_fn;
			if(m_connector_fn)
				m_connector_fn();
			return *this;
		}

		connect_guard& operator= (connect_guard&& move)
		{
			m_connector_fn = std::move(move.m_connector_fn);
			m_disconnector_fn = std::move(move.m_disconnector_fn);
			return *this;
		}

		connect_guard(const connect_guard& copy)
			: m_connector_fn(copy.m_connector_fn)
			, m_disconnector_fn(copy.m_disconnector_fn)
		{
			if(m_connector_fn)
				m_connector_fn();
		}

		connect_guard(connect_guard&& move)
			: m_connector_fn(std::move(move.m_connector_fn))
			, m_disconnector_fn(std::move(move.m_disconnector_fn))
		{}

		~connect_guard()
		{
			disconnect();
		}

		template<typename Connection, typename InstanceOpt, auto ConnectFN, auto DisconnectFN>
		void basic_connect(Connection* connection, InstanceOpt* instance = nullptr)
		{
			disconnect();
			m_connector_fn = [connection, instance](){ ConnectFN(connection, instance); };
			m_disconnector_fn = [connection, instance](){ DisconnectFN(connection, instance); };
			m_connector_fn();
		}

		template<typename Connection, typename InstanceOpt, auto ConnectFN, auto DisconnectFN>
		void basic_connect(Connection& connection, InstanceOpt* instance = nullptr)
		{
			basic_connect<Connection, InstanceOpt, ConnectFN, DisconnectFN>(&connection, instance);
		}

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(entt::dispatcher* connection, InstanceOpt* instance = nullptr);
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(entt::dispatcher& connection, InstanceOpt* instance = nullptr);

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(oe::graphics::IWindow* connection, InstanceOpt* instance = nullptr);
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(oe::graphics::IWindow& connection, InstanceOpt* instance = nullptr);

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(const oe::graphics::Window* connection, InstanceOpt* instance = nullptr);
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(const oe::graphics::Window& connection, InstanceOpt* instance = nullptr);
		
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(oe::utils::GameLoop* connection, InstanceOpt* instance = nullptr);
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(oe::utils::GameLoop& connection, InstanceOpt* instance = nullptr);

		template<typename EventType>
		void connect(entt::dispatcher* connection, const std::function<void(EventType)>& lambda);
		template<typename EventType>
		void connect(entt::dispatcher& connection, const std::function<void(EventType)>& lambda);

		operator bool() const
		{
			return (m_disconnector_fn.operator bool());
		}

		void reset()
		{
			disconnect();
		}

		void disconnect()
		{
			if(m_disconnector_fn)
				m_disconnector_fn();

			m_connector_fn = {};
			m_disconnector_fn = {};
		}
	};
}