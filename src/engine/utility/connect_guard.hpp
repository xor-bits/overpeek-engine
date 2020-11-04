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
	class connect_guard
	{
	public:
		struct data_t { virtual ~data_t() {}; };
		template<typename EventType>
		struct data_t_e : data_t
		{
			const std::function<void(EventType)> fn;

			data_t_e() = default;
			~data_t_e() override = default;

			template<typename Callable>
			data_t_e(Callable&& f) : fn(f) { if(!fn) throw std::runtime_error("Callable was empty"); }

			void call(const EventType& e) const { fn(e); }
		};
		
		template<typename Connection>
		struct connector_disconnector_getter
		{
			template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
			constexpr static auto connector();
			template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
			constexpr static auto disconnector();
		};
	private:
		std::function<void()> m_connector_fn;
		std::function<void()> m_disconnector_fn;
		std::unique_ptr<connect_guard::data_t> m_data;
	
	public:
		[[nodiscard]] connect_guard() noexcept = default;

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

		template<typename EventType, auto ListenerFn, typename InstanceOpt = void, typename Connection>
		void connect(Connection* connection, InstanceOpt* instance = nullptr)
		{
			constexpr auto connector = connector_disconnector_getter<Connection>::template connector<EventType, ListenerFn, InstanceOpt>()();
			constexpr auto disconnector = connector_disconnector_getter<Connection>::template disconnector<EventType, ListenerFn, InstanceOpt>()();
			basic_connect<Connection, InstanceOpt, connector, disconnector>(connection, instance);
		}
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void, typename Connection>
		void connect(Connection& connection, InstanceOpt* instance = nullptr)
		{ connect<EventType, ListenerFn, InstanceOpt, Connection>(&connection, instance); }
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void, typename Connection>
		void connect(Connection* connection, InstanceOpt& instance)
		{ connect<EventType, ListenerFn, InstanceOpt, Connection>(connection, *instance); }
		template<typename EventType, auto ListenerFn, typename InstanceOpt = void, typename Connection>
		void connect(Connection& connection, InstanceOpt& instance)
		{ connect<EventType, ListenerFn, InstanceOpt, Connection>(&connection, *instance); }

		template<typename EventType, typename Connection, typename Lambda>
		void connect(Connection* connection, Lambda&& lambda)
		{
			constexpr auto connector = connector_disconnector_getter<Connection>::template connector<EventType, &data_t_e<EventType>::call, data_t_e<EventType>>()();
			constexpr auto disconnector = connector_disconnector_getter<Connection>::template disconnector<EventType, &data_t_e<EventType>::call, data_t_e<EventType>>()();
			
			disconnect();
			auto data = std::make_unique<data_t_e<EventType>>(lambda);
			m_data = std::move(data);
			auto data_e_ptr = dynamic_cast<data_t_e<EventType>*>(m_data.get());
			
			basic_connect<Connection, data_t_e<EventType>, connector, disconnector>(connection, data_e_ptr);
		}
		template<typename EventType, typename Connection, typename Lambda>
		void connect(Connection& connection, Lambda&& lambda)
		{ connect<EventType, Connection, Lambda>(&connection, std::forward<Lambda>(lambda)); }

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