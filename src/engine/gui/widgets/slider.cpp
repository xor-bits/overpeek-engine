#include "slider.hpp"

#include "engine/engine.hpp"



namespace oe::gui {

	Slider::Slider(glm::ivec2 size) :
		Widget(size),
		m_callback(nullptr),
		m_min(-5.0f),
		m_max(5.0f),
		m_value(0.0f),
		m_sprite(nullptr),
		m_slider_color(0.7f, 0.7f, 0.7f, 1.0f),
		m_bg_color(0.3f, 0.3f, 0.3f, 1.0f),
		m_slider_size(30, size.y),
		m_dragging(false)
	{}

	void Slider::render(oe::graphics::Renderer& renderer) {
		// m_value = sin(oe::utils::Clock::getSessionMillisecond() * 0.001f) * 5.0f;
		renderer.submit(m_render_position, m_size, m_sprite, m_bg_color);

		glm::vec2 slider_pos = glm::vec2(oe::utils::map(m_value, m_min, m_max, 0.0f, m_size.x - m_slider_size.x), 0.0f);
		renderer.submit(m_render_position + slider_pos, m_slider_size, m_sprite, m_slider_color);
	}

	void Slider::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		if (cursor_window.x >= m_render_position.x &&
			cursor_window.x < m_render_position.x + m_size.x &&
			cursor_window.y >= m_render_position.y &&
			cursor_window.y < m_render_position.y + m_size.y)
		{
			// hold
			if (button == oe::mouse_buttons::button_left && action == oe::actions::press)
				m_dragging = true;
		}

		// release
		if (button == oe::mouse_buttons::button_left && action == oe::actions::release)
			m_dragging = false;


		if (m_dragging) {
			m_value = oe::utils::clamp(oe::utils::map(cursor_window.x - m_render_position.x - m_slider_size.x * 0.5f, 0.0f, m_size.x - m_slider_size.x, m_min, m_max), m_min, m_max);
			if (m_callback) m_callback(m_value);
		}
	}

}