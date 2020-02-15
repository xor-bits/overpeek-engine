#include "checkbox.hpp"



namespace oe::gui {

	void Checkbox::checkbox_action(oe::mouse_buttons button, oe::actions action) {
		if (button == oe::mouse_buttons::button_left && action == oe::actions::release) {
			m_toggled = !m_toggled;
			if (m_checkbox_info.callback)
				m_checkbox_info.callback(m_toggled);
		}
	}

	Checkbox::Checkbox(const CheckboxInfo& _checkbox_info)
		: Widget(_checkbox_info.size, _checkbox_info.align_parent, _checkbox_info.align_render, _checkbox_info.offset_position)
		, m_checkbox_info(_checkbox_info)
		, m_toggled(_checkbox_info.initial)
	{
		ButtonInfo button_info;
		button_info.callback = BUTTON_CALLBACK_WRAPPER(checkbox_action);
		button_info.size = _checkbox_info.size;
		button_info.offset_position = { 0, 0 };
		button_info.align_parent = oe::alignments::center_center;
		button_info.align_render = oe::alignments::center_center;
		m_button = new Button(button_info);
		addSubWidget(m_button);
	}

	void Checkbox::render(oe::graphics::Renderer& renderer) {
		renderer.submit(render_position, size, m_checkbox_info.sprite, m_checkbox_info.color_back);

		if (m_toggled) {
			glm::vec2 pos = render_position + size * 0.5f;
			renderer.submit(pos, size * 0.7f, m_checkbox_info.sprite, m_checkbox_info.color_mark, oe::alignments::center_center);
		}
	}

}