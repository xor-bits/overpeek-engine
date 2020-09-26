#pragma once

#include <entt/signal/dispatcher.hpp>
#include <entt/signal/delegate.hpp>
#include <tuple>
#include <functional>



namespace oe::utils
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	void connect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance = nullptr)
	{
		static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
		
		if constexpr(is_void_t)
			dp->sink<EventType>().template connect<ListenerFn>();
		else
			dp->sink<EventType>().template connect<ListenerFn>(instance);
	}

	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	void disconnect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance = nullptr)
	{
		static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
		
		if constexpr(is_void_t)
			dp->sink<EventType>().template disconnect<ListenerFn>();
		else
			dp->sink<EventType>().template disconnect<ListenerFn>(*instance);
	}

	class connect_guard
	{
	private:
		std::function<void()> m_connector_fn;
		std::function<void()> m_disconnector_fn;
	
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
		void connect(Connection* connection, InstanceOpt* instance = nullptr)
		{
			m_connector_fn = [connection, instance](){ ConnectFN(connection, instance); };
			m_disconnector_fn = [connection, instance](){ DisconnectFN(connection, instance); };
			m_connector_fn();
		}

		template<typename Connection, typename InstanceOpt, auto ConnectFN, auto DisconnectFN>
		void connect(Connection& connection, InstanceOpt* instance = nullptr)
		{
			connect<Connection, InstanceOpt, ConnectFN, DisconnectFN>(&connection, instance);
		}

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void, typename Connection> // implement for anything
		void connect(Connection&& connection, InstanceOpt* instance = nullptr);

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(entt::dispatcher* connection, InstanceOpt* instance = nullptr)
		{
			connect<entt::dispatcher, InstanceOpt, connect_dispatcher<EventType,ListenerFn,InstanceOpt>, disconnect_dispatcher<EventType,ListenerFn,InstanceOpt>>(connection, instance);
		}

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
		void connect(entt::dispatcher& connection, InstanceOpt* instance = nullptr)
		{
			connect<entt::dispatcher, InstanceOpt, connect_dispatcher<EventType,ListenerFn,InstanceOpt>, disconnect_dispatcher<EventType,ListenerFn,InstanceOpt>>(connection, instance);
		}

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