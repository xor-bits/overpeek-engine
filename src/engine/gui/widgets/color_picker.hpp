#pragma once

#include "widget.hpp"
#include "vec_slider.hpp"



namespace oe::gui {	class Slider; class SpritePanel; class ColorPickerWheel; class Button; struct ButtonHoverEvent; struct ButtonUseEvent; }
namespace oe::graphics { struct Sprite; }

namespace oe::gui
{
	enum class close_fn {
		never, immediately, leave_bbox, click_out
	};
	enum class open_fn {
		never, immediately, in_bbox, click_in
	};

	struct ColorPickerInfo
	{
		uint8_t draw_value                 = 2; // (false/0) = no draw, (true/1) = draw 0.0-1.0, 2 = draw 0-256
		glm::vec4 initial_color            = oe::colors::red;
		glm::vec4 background_color         = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr;
		bool popup_color_picker_wheel      = true;
		close_fn popup_close               = close_fn::leave_bbox;
		open_fn popup_open                 = open_fn::click_in;

		WidgetInfo widget_info = { { 200, 100 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct ColorPickerHoverEvent
	{};

	struct ColorPickerUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		glm::vec4 value;
	};

	class ColorPicker : public VecSlider<4>
	{
	public:
		ColorPickerInfo m_color_picker_info;
		ColorPickerHoverEvent m_event_hover_latest;
		ColorPickerUseEvent m_event_use_latest;

	private:
		std::shared_ptr<ColorPickerWheel> popup_wheel;
		std::shared_ptr<Button> preview_button;
		std::shared_ptr<SpritePanel> preview_panel;

	public:
		ColorPicker(Widget* parent, GUI& gui_manager, const ColorPickerInfo& color_picker_info = {});
		~ColorPicker();

		const glm::vec4& get() const;
		void set(const glm::vec4& color);
		void update();

		virtual void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_mouse_button(const MouseButtonEvent& e);
		void on_cursor_pos(const CursorPosEvent& e);
		oe::utils::connect_guard m_cg_mouse_button;
		oe::utils::connect_guard m_cg_cursor_pos;
		void on_color_wheel_hover(const ColorPickerHoverEvent& e);
		void on_color_wheel_use(const ColorPickerUseEvent& e);
		void on_vec_slider_hover(const VecSliderHoverEvent<4>& e);
		void on_vec_slider_use(const VecSliderUseEvent<4>& e);
		void on_button_hover(const ButtonHoverEvent& e);
		void on_button_use(const ButtonUseEvent& e);
	};

}