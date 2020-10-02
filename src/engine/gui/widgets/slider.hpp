#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	struct SliderInfo
	{
		float value_initial                                          = 0.0f;
		glm::vec2 value_bounds                                       = { -1.0f, 1.0f };
		float value_steps                                            = 1e5;
		bool interactable                                            = true;
		bool draw_value                                              = false;
		int font_size                                                = 14;
		oe::utils::FontFile font_file                                = {}; // empty for gui default
		std::function<std::u32string(const float&)> draw_format      = &SliderInfo::default_formatter;
		bool vertical                                                = false;
		bool scroll                                                  = false;
		glm::ivec2 knob_size                                         = { 30, 30 };
		glm::vec4 knob_color                                         = oe::colors::grey;
		glm::vec4 slider_lcolor                                      = oe::colors::dark_grey;
		glm::vec4 slider_rcolor                                      = oe::colors::darker_grey;
		bool linear_color                                            = false;
		const oe::graphics::Sprite* knob_sprite                      = nullptr;
		const oe::graphics::Sprite* slider_sprite                    = nullptr;

		static std::u32string default_formatter(const float& val);

		WidgetInfo widget_info                    = { { 150, 30 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct SliderHoverEvent
	{};

	struct SliderUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		float value;
	};

	class Slider : public Widget
	{
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
		SliderInfo m_slider_info;
		float& m_value;
		SliderHoverEvent m_event_hover_latest;
		SliderUseEvent m_event_use_latest;

	public:
		Slider(Widget* parent, GUI& gui_manager, float& value_ref, const SliderInfo& slider_info = {});
		Slider(Widget* parent, GUI& gui_manager, const SliderInfo& slider_info = {}) : Slider(parent, gui_manager, m_slider_info.value_initial, slider_info) {}
		~Slider();

		virtual void virtual_toggle(bool enabled) override;
	
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