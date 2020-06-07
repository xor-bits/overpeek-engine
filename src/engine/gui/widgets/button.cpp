#include "button.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	Button::Button(GUI* gui_manager, const ButtonInfo& _button_info)
		: Widget(gui_manager, _button_info.size, _button_info.align_parent, _button_info.align_render, _button_info.offset_position)
		, button_info(_button_info)
	{
		// event listeners
		m_gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Button::on_button>(this);
	}

	Button::~Button()
	{
		// event listeners
		m_gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Button::on_button>(this);
	}

	void Button::on_button(const MouseButtonEvent& event)
	{
		if (event.cursor_pos.cursor_windowspace.x >= render_position.x && 
			event.cursor_pos.cursor_windowspace.x < render_position.x + size.x && 
			event.cursor_pos.cursor_windowspace.y >= render_position.y && 
			event.cursor_pos.cursor_windowspace.y < render_position.y + size.y) 
		{
			if (button_info.callback) button_info.callback(event.button, event.action);
		}
	}

}