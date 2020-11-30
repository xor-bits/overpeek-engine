#include "checkbox.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/utility/connect_guard_additions.hpp"

#include "button.hpp"



namespace oe::gui
{
	Checkbox::Checkbox(Widget* parent, GUI& gui_manager, const info_t& _checkbox_info, value_t& value_ref)
		: Widget(parent, gui_manager, _checkbox_info.widget_info)
		, m_checkbox_info(_checkbox_info)
		, m_value(value_ref)
	{
		Button::info_t button_info;
		button_info.pixel_size = { 0, 0 };
		button_info.fract_size = { 1.0f, 1.0f };
		m_button = create(button_info);
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
			m_cg_render.connect<GUIRenderEvent, &Checkbox::on_render, Checkbox>(m_gui_manager.m_dispatcher, this);
		}
		else
		{
			quad_check.reset();
			quad_box.reset();

			// event listeners
			m_cg_render.disconnect();
		}
	}

	void Checkbox::on_render(const GUIRenderEvent& /* event */)
	{
		if(!m_cg_render)
			return;

		quad_box->setPosition(m_render_position);
		quad_box->setSize(m_render_size);
		quad_box->setZ(m_z);
		quad_box->setColor(m_checkbox_info.color_back);
		quad_box->setSprite(m_checkbox_info.sprite);

		quad_check->setPosition(static_cast<glm::vec2>(m_render_position + m_render_size / 2));
		quad_check->setSize(m_value ? static_cast<glm::vec2>(m_render_size) * 0.7f : glm::vec2{ 0.0f, 0.0f });
		quad_check->setZ(m_z + 0.05f);
		quad_check->setColor(m_checkbox_info.color_mark);
		quad_check->setSprite(m_checkbox_info.sprite);
		quad_check->setRotationAlignment(oe::alignments::center_center);
	}

	void Checkbox::on_button_use(const ButtonUseEvent& e)
	{
		if (e.button == oe::mouse_buttons::button_left && e.action == oe::actions::release)
			m_value = !m_value;
		
		event_use_latest.action = e.action;
		event_use_latest.button = e.button;
		event_use_latest.modifier = e.modifier;
		event_use_latest.value = m_value;
		m_dispatcher.trigger(event_use_latest);
	}

	void Checkbox::on_button_hover(const ButtonHoverEvent& /* e */)
	{
		m_dispatcher.trigger(event_hover_latest);
	}


}