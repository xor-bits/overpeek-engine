#pragma once

#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/renderer.hpp"
#include "widget.hpp"
#include "event.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/connect_guard_additions.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	template<typename T>
	struct BasicSliderInputHoverEvent : BaseHoverEvent {};
	template<typename T>
	struct BasicSliderInputUseEvent : BaseUseEvent
	{
		T value;
	};



	struct BaseBasicSliderInputInfo_t
	{
		// draw
		bool draw_value                                              = true;
		uint16_t font_size                                           = 14;
		oe::utils::FontFile font_file                                = {}; // empty for gui default
		// visuals
		bool vertical                                                = false;
		glm::ivec2 knob_size                                         = { 30, 30 };
		glm::vec4 knob_color                                         = oe::colors::grey;
		glm::vec4 slider_lcolor                                      = oe::colors::dark_grey;
		glm::vec4 slider_rcolor                                      = oe::colors::darker_grey;
		bool linear_color                                            = false;
		const oe::graphics::Sprite* knob_sprite                      = nullptr;
		const oe::graphics::Sprite* slider_sprite                    = nullptr;
		// io
		interact_type_flags interact_flags                           = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;
		// base
		Widget::Info widget_info                                     = { { 150, 30 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};


	
	class BaseBasicSliderInput : public Widget
	{
	protected:
		BaseBasicSliderInput(Widget* parent, GUI& gui_manager, const BaseBasicSliderInputInfo_t& info)
			: Widget(parent, gui_manager, info.widget_info)
		{}
		~BaseBasicSliderInput() override = default;

		[[nodiscard]] virtual float get_rendered_value() const = 0;
		virtual void set_rendered_value(float) const = 0;
		[[nodiscard]] virtual std::u32string get_rendered_label() const = 0;
		[[nodiscard]] virtual const BaseBasicSliderInputInfo_t& get_info() const = 0;
		[[nodiscard]] virtual BaseHoverEvent& get_hover_event() = 0;
		[[nodiscard]] virtual BaseUseEvent& get_use_event() = 0;
		virtual void clamp() = 0;
		virtual void send_hover_event() = 0;
		virtual void send_use_event() = 0;
		virtual void step(float mult) = 0;
	private:
		bool m_dragging = false;
		oe::graphics::u32TextLabel* value_label = nullptr;
		std::unique_ptr<oe::graphics::Quad> label_quad;
		std::unique_ptr<oe::graphics::Quad> quad_knob;
		std::unique_ptr<oe::graphics::Quad> quad_lslider;
		std::unique_ptr<oe::graphics::Quad> quad_rslider;

	public:
		void virtual_toggle(bool enabled) override;

	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		void on_scroll(const ScrollEvent& event);
		oe::utils::connect_guard m_cg_render;
		oe::utils::connect_guard m_cg_cursor;
		oe::utils::connect_guard m_cg_button;
		oe::utils::connect_guard m_cg_scroll;
	};



	template<typename T>
	class BasicSliderInput : public BaseBasicSliderInput
	{
	public:
		using value_t = T;
		struct info_t : BaseBasicSliderInputInfo_t
		{
			using widget_t = BasicSliderInput<T>;
			using value_t = T;

			// value
			value_t initial_value                                        = static_cast<value_t>(0.0);
			glm::vec<2,value_t> value_bounds                             = { static_cast<value_t>(0.0), static_cast<value_t>(10.0) };
			value_t step_size                                            = static_cast<value_t>(1.0);
			// draw
			std::function<std::u32string(const value_t&)> draw_format          = &default_formatter;
			

		
			//
			[[nodiscard]] static inline std::u32string default_formatter(const value_t& val)
			{
				if constexpr(std::is_floating_point_v<value_t>)
					return fmt::format(U"V: {:.1f}", val);
				else
					return fmt::format(U"V: {}", val);
			}
		};

	protected:
		[[nodiscard]] float get_rendered_value() const override
		{
			return oe::utils::map(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y, static_cast<value_t>(0.0), static_cast<value_t>(1.0));
		}
		
		void set_rendered_value(float val) const override
		{
			m_value = oe::utils::map(val, static_cast<value_t>(0.0), static_cast<value_t>(1.0), m_slider_info.value_bounds.x, m_slider_info.value_bounds.y);
		}
		
		[[nodiscard]] std::u32string get_rendered_label() const override
		{
			return m_slider_info.draw_format(m_value);
		}
		
		[[nodiscard]] const BaseBasicSliderInputInfo_t& get_info() const override
		{
			return static_cast<const BaseBasicSliderInputInfo_t&>(m_slider_info);
		}
		
		[[nodiscard]] BaseHoverEvent& get_hover_event() override
		{
			return static_cast<BaseHoverEvent&>(m_event_hover_latest);	
		}

		[[nodiscard]] BaseUseEvent& get_use_event() override
		{
			return static_cast<BaseUseEvent&>(m_event_use_latest);
		}

		void clamp() override
		{
			m_value = std::clamp(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y);
			
			m_value /= m_slider_info.step_size;
			m_value = std::round(m_value);
			m_value *= m_slider_info.step_size;
		}

		void send_hover_event() override
		{
			m_dispatcher.trigger(m_event_hover_latest);
		}
		
		void send_use_event() override
		{
			m_event_use_latest.value = m_value;
			m_dispatcher.trigger(m_event_use_latest);
		}
		
		void step(float mult) override
		{
			m_value += mult * m_slider_info.step_size;
		}

	public:
		info_t m_slider_info;
		value_t& m_value;
		BasicSliderInputHoverEvent<T> m_event_hover_latest;
		BasicSliderInputUseEvent<T> m_event_use_latest;

	public:
		BasicSliderInput(Widget* parent, GUI& gui_manager, const info_t& slider_info, value_t& value_ref)
			: BaseBasicSliderInput(parent, gui_manager, static_cast<const BaseBasicSliderInputInfo_t&>(slider_info))
			, m_slider_info(slider_info)
			, m_value(value_ref)
		{
			if (!m_slider_info.slider_sprite) m_slider_info.slider_sprite = m_slider_info.knob_sprite;
			if (!m_slider_info.knob_sprite)   m_slider_info.knob_sprite   = m_slider_info.slider_sprite;
		}
		BasicSliderInput(Widget* parent, GUI& gui_manager, const info_t& slider_info)
			: BasicSliderInput(parent, gui_manager, slider_info, m_slider_info.initial_value)
		{}

		~BasicSliderInput() override = default;
	};

	using fSliderInput = BasicSliderInput<float>;
	using dSliderInput = BasicSliderInput<double>;
	using iSliderInput = BasicSliderInput<int32_t>;
	using uSliderInput = BasicSliderInput<uint32_t>;
	using SliderInput = fSliderInput;
}