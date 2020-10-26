#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	struct SliderInputInfo
	{
		float initial_value                                          = 0.0f;
		glm::vec2 value_bounds                                       = { -1.0f, 1.0f };
		float value_steps                                            = 1e5;
		bool draw_value                                              = false;
		std::function<std::u32string(const float&)> draw_format      = &SliderInputInfo::default_formatter;
		int font_size                                                = 14;
		oe::utils::FontFile font_file                                = {}; // empty for gui default
		bool vertical                                                = false;
		glm::ivec2 knob_size                                         = { 30, 30 };
		glm::vec4 knob_color                                         = oe::colors::grey;
		glm::vec4 slider_lcolor                                      = oe::colors::dark_grey;
		glm::vec4 slider_rcolor                                      = oe::colors::darker_grey;
		bool linear_color                                            = false;
		const oe::graphics::Sprite* knob_sprite                      = nullptr;
		const oe::graphics::Sprite* slider_sprite                    = nullptr;
		
		interact_type_flags interact_flags                           = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;

		WidgetInfo widget_info                                       = { { 150, 30 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
		
		//
		static std::u32string default_formatter(const float& val);
	};

	struct SliderInputHoverEvent
	{};

	struct SliderInputUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		float value;
	};

	class SliderInput : public Widget
	{
	public:
		using value_t = float;
		using info_t = SliderInputInfo;

	private:
		oe::graphics::u32TextLabel* value_label;
		std::unique_ptr<oe::graphics::Quad> label_quad;
		std::unique_ptr<oe::graphics::Quad> quad_knob;
		std::unique_ptr<oe::graphics::Quad> quad_lslider;
		std::unique_ptr<oe::graphics::Quad> quad_rslider;

	private:
		bool m_dragging;

		void clamp();

	public:
		SliderInputInfo m_slider_info;
		value_t& m_value;
		SliderInputHoverEvent m_event_hover_latest;
		SliderInputUseEvent m_event_use_latest;

	public:
		SliderInput(Widget* parent, GUI& gui_manager, value_t& value_ref, const SliderInputInfo& slider_info = {});
		SliderInput(Widget* parent, GUI& gui_manager, const SliderInputInfo& slider_info = {}) : SliderInput(parent, gui_manager, m_slider_info.initial_value, slider_info) {}

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

}