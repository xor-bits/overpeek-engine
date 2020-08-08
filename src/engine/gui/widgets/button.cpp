#include "button.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"



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
		gui_manager->getWindow()->connect_listener<oe::CursorPosEvent, &Button::on_cursor>(this);
		gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Button::on_button>(this);

		Widget::managerAssigned(gui_manager);
	}

	void Button::managerUnassigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->getWindow()->disconnect_listener<oe::CursorPosEvent, &Button::on_cursor>(this);
		gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Button::on_button>(this);

		Widget::managerUnassigned(gui_manager);
	}

	bool check_inside(const glm::vec2& point, const glm::vec2& top_left, const glm::vec2& size); // slider.cpp has the definition
		
	void Button::on_cursor(const CursorPosEvent& event)
	{
		if (check_inside(event.cursor_windowspace, render_position, size)) 
		{
			dispatcher.trigger(event_hover_latest);
		}
	}

	void Button::on_button(const MouseButtonEvent& event)
	{
		if (check_inside(event.cursor_pos.cursor_windowspace, render_position, size)) 
		{
			event_use_latest.action = event.action;
			event_use_latest.button = event.button;
			event_use_latest.modifier = event.mods;
			dispatcher.trigger(event_use_latest);
		}
	}

}