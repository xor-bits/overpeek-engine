#include "checkbox.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"

#include "button.hpp"



namespace oe::gui
{
	Checkbox::Checkbox(const CheckboxInfo& _checkbox_info)
		: Widget(_checkbox_info.widget_info)
		, m_checkbox_info(_checkbox_info)
	{
		ButtonInfo button_info;
		button_info.widget_info = { m_info.size, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
		m_button = new Button(button_info);
		addSubWidget(m_button);
	}

	Checkbox::~Checkbox()
	{}

	void Checkbox::managerAssigned(GUI* gui_manager)
	{
		quad_check = gui_manager->getRenderer()->create();
		quad_box = gui_manager->getRenderer()->create(); // check - box, hehe

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Checkbox::on_render>(this);
		m_button->connect_listener<ButtonUseEvent, &Checkbox::on_button_use>(this);
		m_button->connect_listener<ButtonHoverEvent, &Checkbox::on_button_hover>(this);

		Widget::managerAssigned(gui_manager);
	}

	void Checkbox::managerUnassigned(GUI* gui_manager)
	{
		quad_check.reset();
		quad_box.reset();

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Checkbox::on_render>(this);
		m_button->disconnect_listener<ButtonUseEvent, &Checkbox::on_button_use>(this);
		m_button->disconnect_listener<ButtonHoverEvent, &Checkbox::on_button_hover>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void Checkbox::on_render(const GUIRenderEvent& event)
	{
		quad_check->toggle(m_info.toggled);
		quad_box->toggle(m_info.toggled);
		if (!m_info.toggled)
		{
			quad_check->update(quad_check);
			quad_box->update(quad_box);
			return;
		}

		NULL_SPRITE_CHECK(m_checkbox_info.sprite);

		quad_box->setPosition(render_position);
		quad_box->setZ(z);
		quad_box->setSize(m_info.size);
		quad_box->setColor(m_checkbox_info.color_back);
		quad_box->setSprite(m_checkbox_info.sprite);
		quad_box->update(quad_box);

		if (m_checkbox_info.initial) {
			quad_check->setSize(static_cast<glm::vec2>(m_info.size) * 0.7f);
		}
		else
		{
			quad_check->setSize({ 0.0f, 0.0f });
		}
		quad_check->setPosition(static_cast<glm::vec2>(render_position + m_info.size / 2));
		quad_check->setZ(z + 0.05f);
		quad_check->setColor(m_checkbox_info.color_mark);
		quad_check->setSprite(m_checkbox_info.sprite);
		quad_check->setRotationAlignment(oe::alignments::center_center);
		quad_check->update(quad_check);
	}

	void Checkbox::on_button_use(const ButtonUseEvent& e)
	{
		if (e.button == oe::mouse_buttons::button_left && e.action == oe::actions::release) {
			m_checkbox_info.initial = !m_checkbox_info.initial;
		}
		
		event_use_latest.action = e.action;
		event_use_latest.button = e.button;
		event_use_latest.modifier = e.modifier;
		event_use_latest.value = m_checkbox_info.initial;
		dispatcher.trigger(event_use_latest);
	}

	void Checkbox::on_button_hover(const ButtonHoverEvent& e)
	{
		dispatcher.trigger(event_hover_latest);
	}


}