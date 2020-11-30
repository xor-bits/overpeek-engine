#pragma once

#include "engine/enum.hpp"
#include "engine/utility/connect_guard.hpp"
#include "engine/internal_libs.hpp"

#include <unordered_set>

#ifdef ENABLE_NULL_SPRITE_CHECK
#define NULL_SPRITE_CHECK(x) if (!x) { spdlog::warn("No sprite for {}", typeid(*this).name()); return; }
#else
#define NULL_SPRITE_CHECK(x)
#endif /* NULL_SPRITE_CHECK */



namespace oe::gui { struct GUIRenderEvent; struct GUIPreRenderEvent; class GUI; }

namespace oe::gui
{
	struct widget_vec
	{
		glm::ivec2 m_pixel_size;
		glm::vec2 m_fract_size;

		// pixel_size is like px in .css
		// fract_size is like em in .css
		constexpr widget_vec(const glm::ivec2& pixel_size, const glm::vec2& fract_size)
			: m_pixel_size(pixel_size)
			, m_fract_size(fract_size)
		{}

		// pixel_size is like px in .css
		constexpr widget_vec(const glm::ivec2& pixel_size)
			: m_pixel_size(pixel_size)
			, m_fract_size({ 0.0f, 0.0f })
		{}

		// fract_size is like em in .css
		constexpr widget_vec(const glm::vec2& fract_size)
			: m_pixel_size({ 0, 0 })
			, m_fract_size(fract_size)
		{}

		// defaults to 0, 0
		constexpr widget_vec()
			: m_pixel_size({ 0, 0 })
			, m_fract_size({ 0.0f, 0.0f })
		{}

		// somewhat similar to .css
		// ex. 1.0em - 50px, 50px
		// ex. max(1em, 50px), 1em
		constexpr widget_vec(std::string_view parsed)
		{

		}
	};

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		struct Info
		{
			using widget_t = Widget;

			// exact positioning/sizing
			glm::ivec2                 pixel_size = { 50, 50 };
			glm::ivec2        pixel_origon_offset = { 0, 0 };
			// relative positioning/sizing
			glm::vec2                  fract_size = { 0.0f, 0.0f };
			glm::vec2         fract_origon_offset = oe::alignments::top_left; // where to put the origin on the parent widget
			// self alignment
			glm::vec2                align_origon = oe::alignments::top_left; // how to align this widget to the origon
			bool                          toggled = true;
		};
		using info_t = Info;
		
	private:
		Widget* m_parent;
		std::unordered_set<std::shared_ptr<Widget>> m_nodes;
		static inline float s_z_acc = 0.0f;
		
		bool m_toggle_pending = false;
		bool m_toggle_pending_value = false;
		
		void reset_render_pos();


	protected:
		GUI& m_gui_manager;
		float m_z;

	public:
		glm::ivec2 m_render_size = { 0, 0 };
		glm::ivec2 m_render_position = { 0, 0 };
		info_t m_info;
		
		entt::dispatcher m_dispatcher;
		std::vector<oe::utils::connect_guard> m_user_event_cg_guards;

	public:
		Widget(Widget* parent, GUI& gui_manager, const info_t& info);
		virtual ~Widget();

		// connect event
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			m_dispatcher.sink<Event>().template connect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			m_dispatcher.sink<Event>().template connect<Listener>();
		}
		// connect event
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			m_dispatcher.sink<Event>().template disconnect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			m_dispatcher.sink<Event>().template disconnect<Listener>();
		}

		// create subwidget
		template<typename T, typename ... Args>
		std::shared_ptr<typename T::widget_t> create(const T& info, Args& ... args)
		{
			auto ptr = std::make_shared<typename T::widget_t>(this, m_gui_manager, info, args...);
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
		[[nodiscard]] constexpr inline float getZ() const noexcept { return m_z; }
		constexpr inline void overrideZ(float z) noexcept { m_z = z; }
		void addZ(float add_to_z) noexcept;
		
		// must not be toggled from an event
		void toggle(bool enabled = true);
		virtual void virtual_toggle(bool /* enabled */) {};
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
struct fmt::formatter<oe::gui::Widget::Info> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const oe::gui::Widget::Info& i, FormatContext& ctx) {
		return format_to(ctx.out(), "[ s: {}, o: {}, a0: {}, a1: {}, t: {} ]", i.size, i.offset_position, i.align_parent, i.align_render, i.toggled);
	}
};