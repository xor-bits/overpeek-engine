#include "button.hpp"



namespace oe::gui {

	Button::Button(GUI* gui_manager, const ButtonInfo& _button_info)
		: Widget(gui_manager, _button_info.size, _button_info.align_parent, _button_info.align_render, _button_info.offset_position)
		, button_info(_button_info)
	{}

	void Button::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		if (cursor_window.x >= render_position.x && 
			cursor_window.x < render_position.x + size.x && 
			cursor_window.y >= render_position.y && 
			cursor_window.y < render_position.y + size.y) 
		{
			if (button_info.callback) button_info.callback(button, action);
		}
	}

}