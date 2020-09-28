#pragma once

#include "connect_guard.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/gameloop.hpp"
#include "engine/interfacegen.hpp"
#include <entt/signal/dispatcher.hpp>



template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void connect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	
	if constexpr(is_void_t)
		dp->sink<EventType>().template connect<ListenerFn>();
	else
		dp->sink<EventType>().template connect<ListenerFn>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void disconnect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	
	if constexpr(is_void_t)
		dp->sink<EventType>().template disconnect<ListenerFn>();
	else
		dp->sink<EventType>().template disconnect<ListenerFn>(*instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void connect_window(oe::graphics::IWindow* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->connect_listener<EventType, ListenerFn>();
	else
		win->connect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void disconnect_window(oe::graphics::IWindow* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->disconnect_listener<EventType, ListenerFn>();
	else
		win->disconnect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void connect_gameloop(oe::utils::GameLoop* gl, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		gl->connect_listener<EventType, ListenerFn>();
	else
		gl->connect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
void disconnect_gameloop(oe::utils::GameLoop* gl, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		gl->disconnect_listener<EventType, ListenerFn>();
	else
		gl->disconnect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}



template<typename EventType>
struct connect_guard_data_t_e : oe::utils::connect_guard_data_t {
	std::function<void(EventType)> fn;
	void call(const EventType& e) { fn(e); }
	~connect_guard_data_t_e() {}
};

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(entt::dispatcher* connection, InstanceOpt* instance)
{
	basic_connect<entt::dispatcher, InstanceOpt, connect_dispatcher<EventType,ListenerFn,InstanceOpt>, disconnect_dispatcher<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(entt::dispatcher& connection, InstanceOpt* instance)
{
	basic_connect<entt::dispatcher, InstanceOpt, connect_dispatcher<EventType,ListenerFn,InstanceOpt>, disconnect_dispatcher<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}

template<typename EventType>
void oe::utils::connect_guard::connect(entt::dispatcher* connection, const std::function<void(EventType)>& lambda)
{
	auto _data = std::make_unique<connect_guard_data_t_e<EventType>>();
	_data->fn = lambda;
	data = std::move(_data);

	disconnect();
	m_connector_fn = [connection, this](){ spdlog::debug("connect lambda"); connection->sink<EventType>().template connect<&connect_guard_data_t_e<EventType>::call>(*dynamic_cast<connect_guard_data_t_e<EventType>*>(data.get())); };
	m_disconnector_fn = [connection, this](){ spdlog::debug("disconnect lambda"); connection->sink<EventType>().template disconnect<&connect_guard_data_t_e<EventType>::call>(*dynamic_cast<connect_guard_data_t_e<EventType>*>(data.get())); };
	m_connector_fn();
}

template<typename EventType>
void oe::utils::connect_guard::connect(entt::dispatcher& connection, const std::function<void(EventType)>& lambda)
{
	connect<EventType>(&connection, lambda);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(oe::graphics::IWindow* connection, InstanceOpt* instance)
{
	basic_connect<oe::graphics::IWindow, InstanceOpt, connect_window<EventType,ListenerFn,InstanceOpt>, disconnect_window<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(oe::graphics::IWindow& connection, InstanceOpt* instance)
{
	basic_connect<oe::graphics::IWindow, InstanceOpt, connect_window<EventType,ListenerFn,InstanceOpt>, disconnect_window<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(const oe::graphics::Window* connection, InstanceOpt* instance)
{
	connect<EventType,ListenerFn,InstanceOpt>(connection->get(), instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(const oe::graphics::Window& connection, InstanceOpt* instance)
{
	connect<EventType,ListenerFn,InstanceOpt>(connection.get(), instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(oe::utils::GameLoop* connection, InstanceOpt* instance)
{
	basic_connect<entt::dispatcher, InstanceOpt, connect_gameloop<EventType,ListenerFn,InstanceOpt>, disconnect_gameloop<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void oe::utils::connect_guard::connect(oe::utils::GameLoop& connection, InstanceOpt* instance)
{
	basic_connect<entt::dispatcher, InstanceOpt, connect_gameloop<EventType,ListenerFn,InstanceOpt>, disconnect_gameloop<EventType,ListenerFn,InstanceOpt>>(connection, instance);
}