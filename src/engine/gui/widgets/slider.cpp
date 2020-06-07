#include "slider.hpp"

#include "engine/engine.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	Slider::Slider(GUI* gui_manager, const SliderInfo& _slider_info) 
		: Widget(gui_manager, _slider_info.slider_size, _slider_info.align_parent, _slider_info.align_render, _slider_info.offset_position)
		, slider_info(_slider_info)
		, m_dragging(false)
	{
		if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		
		if (slider_info.draw_value) {
			value_label = new oe::graphics::TextLabel();
			label_quad = m_gui_manager->getLateRenderer()->createQuad();
		}
		quad_knob = m_gui_manager->getRenderer()->createQuad();
		quad_lslider = m_gui_manager->getRenderer()->createQuad();
		quad_rslider = m_gui_manager->getRenderer()->createQuad();

		// event listeners
		m_gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		m_gui_manager->getWindow()->connect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Slider::on_render>(this);
	}

	Slider::~Slider()
	{
		if (slider_info.draw_value) {
			delete value_label;
			m_gui_manager->getLateRenderer()->destroyQuad(label_quad);
		}
		m_gui_manager->getRenderer()->destroyQuad(quad_knob);
		m_gui_manager->getRenderer()->destroyQuad(quad_lslider);
		m_gui_manager->getRenderer()->destroyQuad(quad_rslider);

		// event listeners
		m_gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		m_gui_manager->getWindow()->disconnect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Slider::on_render>(this);
	}
	
	void Slider::on_render(const GUIRenderEvent& event)
	{
		glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.initial_value, slider_info.min_value, slider_info.max_value, 0.0f, size.x - slider_info.knob_size.x), 0.0f);
		
		{
			*event.z += 1.0f;
			quad_lslider->setPosition(render_position);
			quad_lslider->setZ(*event.z);
			quad_lslider->setSize({ slider_pos.x, size.y });
			quad_lslider->setColor(slider_info.slider_lcolor);
			quad_lslider->setSprite(slider_info.slider_sprite);
			quad_lslider->update();
		}
		{
			*event.z += 1.0f;
			quad_rslider->setPosition({ render_position.x + slider_pos.x, render_position.y });
			quad_rslider->setZ(*event.z);
			quad_rslider->setSize({ size.x - slider_pos.x, size.y });
			quad_rslider->setColor(slider_info.slider_rcolor);
			quad_rslider->setSprite(slider_info.slider_sprite);
			quad_rslider->update();
		}

		*event.z += 1.0f;
		quad_knob->setPosition(render_position + slider_pos + glm::vec2(slider_info.knob_size.x, slider_info.slider_size.y) * 0.5f);
		quad_knob->setZ(*event.z);
		quad_knob->setSize(slider_info.knob_size);
		quad_knob->setColor(slider_info.knob_color);
		quad_knob->setSprite(slider_info.knob_sprite);
		quad_knob->setRotationAlignment(oe::alignments::center_center);
		quad_knob->update();

		if (slider_info.draw_value) {
			*event.z += 1.0f;
			glm::vec2 text_size = glm::vec2(size.y * 0.6f);
			value_label->generate(fmt::format("{:.2f}", slider_info.initial_value), m_gui_manager->getWindow(), { 0.0f, 0.0f, 0.0f, 0.2f });
			label_quad->setPosition(render_position + size * 0.5f);
			label_quad->setZ(*event.z);
			label_quad->setSize(text_size * value_label->getSize());
			label_quad->setSprite(value_label->getSprite());
			label_quad->setColor(oe::colors::white);
			label_quad->setRotationAlignment(oe::alignments::center_center);
			label_quad->update();
		}
	}

	void Slider::on_cursor(const CursorPosEvent& event)
	{
		if (m_dragging) {
			slider_info.initial_value = 
				oe::utils::clamp(oe::utils::map(
					event.cursor_windowspace.x - render_position.x - slider_info.knob_size.x * 0.5f, 
					0.0f, 
					size.x - slider_info.knob_size.x, 
					slider_info.min_value, 
					slider_info.max_value
				), slider_info.min_value, slider_info.max_value);

			if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		}
	}

	void Slider::on_button(const MouseButtonEvent& event)
	{
		if (event.cursor_pos.cursor_windowspace.x >= render_position.x &&
			event.cursor_pos.cursor_windowspace.x < render_position.x + size.x &&
			event.cursor_pos.cursor_windowspace.y >= render_position.y &&
			event.cursor_pos.cursor_windowspace.y < render_position.y + size.y)
		{
			// hold
			if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::press)
				m_dragging = true;
		}

		// release
		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::release)
			m_dragging = false;
	}

}