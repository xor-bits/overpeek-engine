#include "checkbox.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"

#include "button.hpp"



namespace oe::gui
{
	Checkbox::Checkbox(Widget* parent, GUI& gui_manager, const CheckboxInfo& _checkbox_info)
		: Widget(parent, gui_manager, _checkbox_info.widget_info)
		, m_checkbox_info(_checkbox_info)
	{
		ButtonInfo button_info;
		button_info.widget_info = { m_info.size, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
		m_button = create<Button>(button_info);
		m_button->connect_listener<ButtonUseEvent, &Checkbox::on_button_use>(this);
		m_button->connect_listener<ButtonHoverEvent, &Checkbox::on_button_hover>(this);
	}

	Checkbox::~Checkbox()
	{
		m_button->disconnect_listener<ButtonUseEvent, &Checkbox::on_button_use>(this);
		m_button->disconnect_listener<ButtonHoverEvent, &Checkbox::on_button_hover>(this);
	}
	
	void Checkbox::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			quad_check = m_gui_manager.getRenderer()->create();
			quad_box = m_gui_manager.getRenderer()->create(); // check - box, hehe

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &Checkbox::on_render, Checkbox>(m_gui_manager.dispatcher, this);
		}
		else
		{
			quad_check.reset();
			quad_box.reset();

			// event listeners
			m_cg_render.disconnect();
		}
	}

	void Checkbox::on_render(const GUIRenderEvent& event)
	{
		if(!m_cg_render)
			return;

		quad_box->setPosition(render_position);
		quad_box->setZ(z);
		quad_box->setSize(m_info.size);
		quad_box->setColor(m_checkbox_info.color_back);
		quad_box->setSprite(m_checkbox_info.sprite);
		m_gui_manager.update(quad_box->update());

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
		m_gui_manager.update(quad_check->update());
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