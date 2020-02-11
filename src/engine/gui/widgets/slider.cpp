#include "slider.hpp"

#include "engine/engine.hpp"



namespace oe::gui {

	Slider::Slider(const SliderInfo& _slider_info) 
		: Widget(_slider_info.slider_size, _slider_info.align_parent, _slider_info.align_render, _slider_info.offset_position)
		, slider_info(_slider_info)
		, m_dragging(false)
	{
		if (slider_info.callback) slider_info.callback(slider_info.initial_value);
	}

	void Slider::render(oe::graphics::Renderer& renderer) {
		renderer.submit(render_position, size, slider_info.slider_sprite, slider_info.slider_color);

		glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.initial_value, slider_info.min_value, slider_info.max_value, 0.0f, size.x - slider_info.knob_size.x), 0.0f);
		renderer.submit(render_position + slider_pos + glm::vec2(slider_info.knob_size.x, slider_info.slider_size.y) * 0.5f, slider_info.knob_size, slider_info.knob_sprite, slider_info.knob_color, oe::alignments::center_center);
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