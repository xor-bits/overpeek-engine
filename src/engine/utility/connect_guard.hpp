#pragma once

#include <entt/signal/dispatcher.hpp>
#include <tuple>
#include <functional>



namespace oe::utils
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	class connect_guard
	{
	private:
		entt::dispatcher* m_dispatcher = nullptr;
		InstanceOpt* m_instance = nullptr;

	
	public:
		static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;

		connect_guard() = default;

		connect_guard& operator= (const connect_guard& copy)
		{
			m_dispatcher = copy.m_dispatcher;
			m_instance = copy.m_instance;
			if constexpr(is_void_t)
				m_dispatcher->sink<EventType>().template connect<ListenerFn>();
			else
				m_dispatcher->sink<EventType>().template connect<ListenerFn>(m_instance);
			return *this;
		}

		connect_guard& operator= (connect_guard&& move)
		{
			m_dispatcher = move.m_dispatcher;
			m_instance = move.m_instance;
			move.m_dispatcher = nullptr;
			move.m_instance = {};
			return *this;
		}

		connect_guard(const connect_guard& copy)
			: m_dispatcher(copy.m_dispatcher)
			, m_instance(move.m_instance)
		{
			if constexpr(is_void_t)
				m_dispatcher->sink<EventType>().template connect<ListenerFn>();
			else
				m_dispatcher->sink<EventType>().template connect<ListenerFn>(*m_instance);
		}

		connect_guard(connect_guard&& move)
			: m_dispatcher(move.m_dispatcher)
			, m_instance(move.m_instance)
		{
			move.m_dispatcher = nullptr;
			move.m_instance = {};
		}

		connect_guard(entt::dispatcher& dispatcher, InstanceOpt* instance)
			: m_dispatcher(&dispatcher)
			, m_instance(instance)
		{
			if constexpr(is_void_t)
				m_dispatcher->sink<EventType>().template connect<ListenerFn>();
			else
				m_dispatcher->sink<EventType>().template connect<ListenerFn>(*m_instance);
		}

		~connect_guard()
		{
			if(m_dispatcher)
			{
				if constexpr(is_void_t)
					m_dispatcher->sink<EventType>().template disconnect<ListenerFn>();
				else
					m_dispatcher->sink<EventType>().template disconnect<ListenerFn>(*m_instance);
			}

			m_dispatcher = nullptr;
		}

		void reset()
		{
			connect_guard::~connect_guard();
		}
	};
}