#include "checkbox.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui {

	void Checkbox::checkbox_action(oe::mouse_buttons button, oe::actions action) {
		if (button == oe::mouse_buttons::button_left && action == oe::actions::release) {
			m_checkbox_info.initial = !m_checkbox_info.initial;
			if (m_checkbox_info.callback)
				m_checkbox_info.callback(m_checkbox_info.initial);
		}
	}

	Checkbox::Checkbox(const CheckboxInfo& _checkbox_info)
		: Widget(_checkbox_info.size, _checkbox_info.align_parent, _checkbox_info.align_render, _checkbox_info.offset_position)
		, m_checkbox_info(_checkbox_info)
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

	Checkbox::~Checkbox()
	{}

	void Checkbox::managerAssigned(GUI* gui_manager)
	{
		quad_check = gui_manager->getRenderer()->createQuad();
		quad_box = gui_manager->getRenderer()->createQuad(); // check - box, hehe

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Checkbox::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void Checkbox::managerUnassigned(GUI* gui_manager)
	{
		gui_manager->getRenderer()->destroyQuad(quad_check);
		gui_manager->getRenderer()->destroyQuad(quad_box);

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Checkbox::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void Checkbox::on_render(const GUIRenderEvent& event)
	{
		*event.z += 1.0f;
		quad_box->setPosition(render_position);
		quad_box->setZ(*event.z);
		quad_box->setSize(size);
		quad_box->setColor(m_checkbox_info.color_back);
		quad_box->setSprite(m_checkbox_info.sprite);
		quad_box->update();

		if (m_checkbox_info.initial) {
			*event.z += 1.0f;
			quad_check->setPosition(static_cast<glm::vec2>(render_position + size / 2));
			quad_check->setZ(*event.z);
			quad_check->setSize(static_cast<glm::vec2>(size) * 0.7f);
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