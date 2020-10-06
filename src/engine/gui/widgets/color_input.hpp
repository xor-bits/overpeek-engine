#pragma once

#include "widget.hpp"



namespace oe::gui {	class SliderInput; class SpritePanel; class ColorPicker; class Button; struct ButtonHoverEvent; struct ButtonUseEvent; }
namespace oe::graphics { struct Sprite; }

namespace oe::gui
{
	enum class close_fn {
		never, immediately, leave_bbox, click_out
	};
	enum class open_fn {
		never, immediately, in_bbox, click_in
	};
	enum class input_type {
		dragger, slider, none
	};

	struct ColorInputInfo
	{
		glm::vec4 initial_color            = oe::colors::red;
		uint8_t draw_value                 = 2; // (false/0) = no draw, (true/1) = draw 0.0-1.0, 2 = draw 0-256
		glm::vec4 background_color         = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr;

		input_type primary_input           = input_type::slider;

		bool popup_color_picker            = true;
		close_fn popup_close               = close_fn::leave_bbox;
		open_fn popup_open                 = open_fn::click_in;

		WidgetInfo widget_info = { { 200, 100 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct ColorInputHoverEvent
	{};

	struct ColorInputUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		glm::vec4 value;
	};

	using ColorPickerHoverEvent = ColorInputHoverEvent;
	using ColorPickerUseEvent = ColorInputUseEvent;

	class ColorInput : public Widget
	{
    public:
		using value_t = glm::vec4;
		using info_t = ColorInputInfo;
		
	public:
		ColorInputInfo m_color_input_info;
		value_t& m_value;
		ColorInputHoverEvent m_event_hover_latest;
		ColorInputUseEvent m_event_use_latest;

	private:
		std::shared_ptr<ColorPicker> m_popup_picker;
		std::shared_ptr<Button> m_preview_button;
		std::shared_ptr<SpritePanel> m_preview_panel;
		
		value_t m_value_last;

	public:
		ColorInput(Widget* parent, GUI& gui_manager, value_t& value_ref, const ColorInputInfo& color_input_info = {});
		ColorInput(Widget* parent, GUI& gui_manager, const ColorInputInfo& color_input_info = {})
			: ColorInput(parent, gui_manager, m_color_input_info.initial_color, color_input_info)
		{}
		~ColorInput();

		virtual void virtual_toggle(bool enabled) override;
	
	private:
		void update();
		// events
		void on_render(const GUIRenderEvent& event);
		void on_mouse_button(const MouseButtonEvent& e);
		void on_cursor_pos(const CursorPosEvent& e);
		oe::utils::connect_guard m_cg_render;
		oe::utils::connect_guard m_cg_mouse_button;
		oe::utils::connect_guard m_cg_cursor_pos;
		void on_color_picker_hover(const ColorPickerHoverEvent& e);
		void on_color_picker_use(const ColorPickerUseEvent& e);
		void on_button_hover(const ButtonHoverEvent& e);
		void on_button_use(const ButtonUseEvent& e);
		
		oe::utils::connect_guard m_cg_vec_use[4];
		oe::utils::connect_guard m_cg_vec_hover[4];
	};

}