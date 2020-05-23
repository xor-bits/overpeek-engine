#include "checkbox.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui {

	void Checkbox::checkbox_action(oe::mouse_buttons button, oe::actions action) {
		if (button == oe::mouse_buttons::button_left && action == oe::actions::release) {
			m_toggled = !m_toggled;
			if (m_checkbox_info.callback)
				m_checkbox_info.callback(m_toggled);
		}
	}

	Checkbox::Checkbox(GUI* gui_manager, const CheckboxInfo& _checkbox_info)
		: Widget(gui_manager, _checkbox_info.size, _checkbox_info.align_parent, _checkbox_info.align_render, _checkbox_info.offset_position)
		, m_checkbox_info(_checkbox_info)
		, m_toggled(_checkbox_info.initial)
	{
		ButtonInfo button_info;
		button_info.callback = BUTTON_CALLBACK_WRAPPER(checkbox_action);
		button_info.size = _checkbox_info.size;
		button_info.offset_position = { 0, 0 };
		button_info.align_parent = oe::alignments::center_center;
		button_info.align_render = oe::alignments::center_center;
		m_button = new Button(gui_manager, button_info);
		addSubWidget(m_button);
		
		quad_check = m_gui_manager->getRenderer()->createQuad();
		quad_box = m_gui_manager->getRenderer()->createQuad(); // hehe
	}

	Checkbox::~Checkbox()
	{
		m_gui_manager->getRenderer()->destroyQuad(quad_check);
		m_gui_manager->getRenderer()->destroyQuad(quad_box); // hehe
	}

	void Checkbox::render(float& z, oe::graphics::Renderer* renderer) {
		quad_box->setPosition(render_position);
		quad_box->setZ(z);
		quad_box->setSize(size);
		quad_box->setColor(m_checkbox_info.color_back);
		quad_box->setSprite(m_checkbox_info.sprite);
		quad_box->update();

		if (m_toggled) {
			z += 1.0f;
			quad_check->setPosition(render_position + size * 0.5f);
			quad_check->setZ(z);
			quad_check->setSize(size * 0.7f);
			quad_check->setColor(m_checkbox_info.color_mark);
			quad_check->setSprite(m_checkbox_info.sprite);
			quad_check->setRotationAlignment(oe::alignments::center_center);
			quad_check->update();
		}
		else
		{
			quad_check->setSize({ 0.0f, 0.0f });
			quad_check->update();
		}
	}

}