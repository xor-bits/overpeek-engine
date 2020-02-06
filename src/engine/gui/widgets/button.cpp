#include "button.hpp"



namespace oe::gui {

	Button::Button(glm::ivec2 size) :
		Widget(size),
		m_callback(nullptr)
	{}

	void Button::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		if (cursor_window.x >= m_render_position.x && 
			cursor_window.x < m_render_position.x + m_size.x && 
			cursor_window.y >= m_render_position.y && 
			cursor_window.y < m_render_position.y + m_size.y) 
		{
			if (m_callback) m_callback(button, action);
		}
	}

}