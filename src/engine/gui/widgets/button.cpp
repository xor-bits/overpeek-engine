#include "button.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	Button::Button(const ButtonInfo& _button_info)
		: Widget(_button_info.size, _button_info.align_parent, _button_info.align_render, _button_info.offset_position)
		, button_info(_button_info)
	{}

	Button::~Button()
	{}

	void Button::managerAssigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Button::on_button>(this);

		Widget::managerAssigned(gui_manager);
	}

	void Button::managerUnassigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Button::on_button>(this);

		Widget::managerUnassigned(gui_manager);
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