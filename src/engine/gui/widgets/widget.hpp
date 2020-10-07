#pragma once

#include "engine/enum.hpp"
#include "engine/utility/connect_guard.hpp"
#include <entt/entt.hpp>
#include <fmt/format.h>

#include <unordered_set>

#ifdef ENABLE_NULL_SPRITE_CHECK
#define NULL_SPRITE_CHECK(x) if (!x) { spdlog::warn("No sprite for {}", typeid(*this).name()); return; }
#else
#define NULL_SPRITE_CHECK(x)
#endif /* NULL_SPRITE_CHECK */



namespace oe::gui { struct GUIRenderEvent; struct GUIPreRenderEvent; class GUI; }

namespace oe::gui
{
	enum interact_type_flags
	{
		none = 0<<0, cursor = 1<<0, scroll = 1<<1, keyboard = 1<<2
	};

	[[nodiscard]] constexpr inline interact_type_flags operator|(interact_type_flags a, interact_type_flags b)
	{
		return static_cast<interact_type_flags>(static_cast<int>(a) | static_cast<int>(b));
	}

	[[nodiscard]] constexpr inline interact_type_flags operator&(interact_type_flags a, interact_type_flags b)
	{
		return static_cast<interact_type_flags>(static_cast<int>(a) & static_cast<int>(b));
	}

	struct WidgetInfo
	{
		glm::ivec2 size                    = { 50, 50 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		bool toggled                       = true;
	};

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		using info_t = WidgetInfo;
		
	private:
		Widget* m_parent;
		std::unordered_set<std::shared_ptr<Widget>> m_nodes;
		static float z_acc;
		
		bool toggle_pending = false;
		bool toggle_pending_value = false;

		std::vector<oe::utils::connect_guard> m_user_event_cg_guards;

	protected:
		GUI& m_gui_manager;
		float z;

	public:
		glm::ivec2 topleft_position = { 0, 0 };
		glm::ivec2 render_position = { 0, 0 };
		WidgetInfo m_info;
		entt::dispatcher dispatcher;

	public:
		Widget(Widget* parent, GUI& gui_manager, const WidgetInfo& info = {});
		virtual ~Widget();

		// connect event
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template connect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			dispatcher.sink<Event>().template connect<Listener>();
		}
		// connect event
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template disconnect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher.sink<Event>().template disconnect<Listener>();
		}

		// create subwidget
		template<typename T, typename ... Args>
		std::shared_ptr<T> create(Args& ... args)
		{
			auto ptr = std::make_shared<T>(this, m_gui_manager, args...);
			(*m_nodes.insert(ptr).first)->base_toggle(ptr->m_info.toggled);
			return ptr;
		}

		// remove subwidget
		template<typename T>
		void remove(const std::shared_ptr<T>& widget)
		{
			m_nodes.erase(widget);
		}

		// remove all subwidgets
		void clear();

		// refrence will be invalidated after the next call to create_event_cg or clear_event_cg
		[[nodiscard]] oe::utils::connect_guard& create_event_cg();
		void clear_event_cg() noexcept;

		// for internal render sorting
		[[nodiscard]] constexpr inline float getZ() const noexcept { return z; }
		constexpr inline void overrideZ(float _z) noexcept { z = _z; }
		void addZ(float add_to_z) noexcept;
		
		// must not be toggled from an event
		void toggle(bool enabled = true);
		virtual void virtual_toggle(bool enabled) {};
		void base_toggle(bool enabled);
		inline Widget* getParent() const { return m_parent; }

	protected:
		// events
		void on_pre_render(const GUIPreRenderEvent& event);
		oe::utils::connect_guard m_cg_pre_render;

	friend class GUI;
	};
}

template <>
struct fmt::formatter<oe::gui::WidgetInfo> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const oe::gui::WidgetInfo& i, FormatContext& ctx) {
		return format_to(ctx.out(), "[ s: {}, o: {}, a0: {}, a1: {}, t: {} ]", i.size, i.offset_position, i.align_parent, i.align_render, i.toggled);
	}
};