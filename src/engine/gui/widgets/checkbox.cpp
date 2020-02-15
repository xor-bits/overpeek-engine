#include "checkbox.hpp"



namespace oe::gui {

	void checkbox_action() {

	}

	Checkbox::Checkbox(const CheckboxInfo& _checkbox_info)
		: Widget(_checkbox_info.size, _checkbox_info.align_parent, _checkbox_info.align_render, _checkbox_info.offset_position)
		, m_checkbox_info(_checkbox_info)
	{
		ButtonInfo button_info;
		button_info.callback = [&](oe::mouse_buttons button, oe::actions action) { m_toggled = !m_toggled; if (m_checkbox_info.callback) m_checkbox_info.callback(m_toggled); };
		button_info.size = _checkbox_info.size;
		button_info.offset_position = _checkbox_info.offset_position;
		button_info.align_parent = _checkbox_info.align_parent;
		button_info.align_render = _checkbox_info.align_render;
		m_button = new Button(button_info);
	}

	void Checkbox::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		m_button->cursor(button, action, cursor_window);
	}

	void Checkbox::render(oe::graphics::Renderer& renderer) {
		renderer.submit(render_position, size, sprite_panel_info.sprite, sprite_panel_info.color);
	}

}