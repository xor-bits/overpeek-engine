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
			label_quad = m_gui_manager->getRenderer()->createQuad();
		}
		quad_knob = m_gui_manager->getRenderer()->createQuad();
		quad_slider = m_gui_manager->getRenderer()->createQuad();
	}

	Slider::~Slider()
	{
		if (slider_info.draw_value) {
			delete value_label;
			m_gui_manager->getRenderer()->destroyQuad(label_quad);
		}
		m_gui_manager->getRenderer()->destroyQuad(quad_knob);
		m_gui_manager->getRenderer()->destroyQuad(quad_slider);
	}

	void Slider::render(float& z, oe::graphics::Renderer* renderer) {
		quad_slider->setPosition(render_position);
		quad_slider->setZ(z);
		quad_slider->setSize(size);
		quad_slider->setColor(slider_info.slider_color);
		quad_slider->setSprite(slider_info.slider_sprite);
		quad_slider->update();

		z += 1.0f;
		glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.initial_value, slider_info.min_value, slider_info.max_value, 0.0f, size.x - slider_info.knob_size.x), 0.0f);
		quad_knob->setPosition(render_position + slider_pos + glm::vec2(slider_info.knob_size.x, slider_info.slider_size.y) * 0.5f);
		quad_knob->setZ(z);
		quad_knob->setSize(slider_info.knob_size);
		quad_knob->setColor(slider_info.knob_color);
		quad_knob->setSprite(slider_info.knob_sprite);
		quad_knob->setRotationAlignment(oe::alignments::center_center);
		quad_knob->update();

		if (slider_info.draw_value) {
			z += 1.0f;
			glm::vec2 text_size = glm::vec2(size.y * 0.6f);
			value_label->generate(fmt::format("{:.2f}", slider_info.initial_value), m_gui_manager->getWindow());
			label_quad->setPosition(render_position + size * 0.5f);
			label_quad->setZ(z);
			label_quad->setSize({ text_size.x * value_label->getAspect(), text_size.y });
			label_quad->setSprite(value_label->getSprite());
			label_quad->setColor(oe::colors::white);
			label_quad->setRotationAlignment(oe::alignments::center_center);
			label_quad->update();
		}
	}

	void Slider::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		if (cursor_window.x >= render_position.x &&
			cursor_window.x < render_position.x + size.x &&
			cursor_window.y >= render_position.y &&
			cursor_window.y < render_position.y + size.y)
		{
			// hold
			if (button == oe::mouse_buttons::button_left && action == oe::actions::press)
				m_dragging = true;
		}

		// release
		if (button == oe::mouse_buttons::button_left && action == oe::actions::release)
			m_dragging = false;


		if (m_dragging) {
			slider_info.initial_value = 
				oe::utils::clamp(oe::utils::map(
					cursor_window.x - render_position.x - slider_info.knob_size.x * 0.5f, 
					0.0f, 
					size.x - slider_info.knob_size.x, 
					slider_info.min_value, 
					slider_info.max_value
				), slider_info.min_value, slider_info.max_value);

			if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		}
	}

}