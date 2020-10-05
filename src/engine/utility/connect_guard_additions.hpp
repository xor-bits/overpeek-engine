#pragma once

#include "connect_guard.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/gameloop.hpp"
#include "engine/interfacegen.hpp"
#include <entt/signal/dispatcher.hpp>



template<typename EventType, auto ListenerFn, typename InstanceOpt>
void connect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	
	if constexpr(is_void_t)
		dp->sink<EventType>().template connect<ListenerFn>();
	else
		dp->sink<EventType>().template connect<ListenerFn>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void disconnect_dispatcher(entt::dispatcher* dp, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	
	if constexpr(is_void_t)
		dp->sink<EventType>().template disconnect<ListenerFn>();
	else
		dp->sink<EventType>().template disconnect<ListenerFn>(*instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void connect_window(oe::graphics::Window* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->get()->connect_listener<EventType, ListenerFn>();
	else
		win->get()->connect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void disconnect_window(oe::graphics::Window* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->get()->disconnect_listener<EventType, ListenerFn>();
	else
		win->get()->disconnect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void connect_iwindow(oe::graphics::IWindow* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->connect_listener<EventType, ListenerFn>();
	else
		win->connect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void disconnect_iwindow(oe::graphics::IWindow* win, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		win->disconnect_listener<EventType, ListenerFn>();
	else
		win->disconnect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void connect_gameloop(oe::utils::GameLoop* gl, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		gl->connect_listener<EventType, ListenerFn>();
	else
		gl->connect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}

template<typename EventType, auto ListenerFn, typename InstanceOpt>
void disconnect_gameloop(oe::utils::GameLoop* gl, InstanceOpt* instance)
{
	static constexpr bool is_void_t = std::is_same_v<InstanceOpt, void>;
	if constexpr(is_void_t)
		gl->disconnect_listener<EventType, ListenerFn>();
	else
		gl->disconnect_listener<EventType, ListenerFn, InstanceOpt>(instance);
}



template<>
struct oe::utils::connect_guard::connector_disconnector_getter<entt::dispatcher>
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto connector() { return [](){ return connect_dispatcher<EventType, ListenerFn, InstanceOpt>; }; }
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto disconnector() { return [](){ return disconnect_dispatcher<EventType, ListenerFn, InstanceOpt>; }; }
};
template<>
struct oe::utils::connect_guard::connector_disconnector_getter<oe::graphics::Window>
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto connector() { return [](){ return connect_window<EventType, ListenerFn, InstanceOpt>; }; }
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto disconnector() { return [](){ return disconnect_window<EventType, ListenerFn, InstanceOpt>; }; }
};
template<>
struct oe::utils::connect_guard::connector_disconnector_getter<oe::graphics::IWindow>
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto connector() { return [](){ return connect_window<EventType, ListenerFn, InstanceOpt>; }; }
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto disconnector() { return [](){ return disconnect_window<EventType, ListenerFn, InstanceOpt>; }; }
};
template<>
struct oe::utils::connect_guard::connector_disconnector_getter<oe::utils::GameLoop>
{
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto connector() { return [](){ return connect_gameloop<EventType, ListenerFn, InstanceOpt>; }; }
	template<typename EventType, auto ListenerFn, typename InstanceOpt = void>
	constexpr static auto disconnector() { return [](){ return disconnect_gameloop<EventType, ListenerFn, InstanceOpt>; }; }
};