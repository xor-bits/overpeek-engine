#include "button.h"

#if _DEBUG
#define _DEFAULT_COLOR glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)
#else
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG


namespace oe::gui {

	Button::Button(glm::ivec2 size) :
		Widget(size),
		m_color(_DEFAULT_COLOR),
		m_callback(nullptr)
	{}

	void Button::render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		renderer.submit(m_render_position, m_size, 0, m_color);
	}

	void Button::cursor(int button, int action, const glm::vec2& cursor_window) {
		if (cursor_window.x >= m_render_position.x && 
			cursor_window.x < m_render_position.x + m_size.x && 
			cursor_window.y >= m_render_position.y && 
			cursor_window.y < m_render_position.y + m_size.y) 
		{
			if (m_callback) m_callback(button, action);
		}
	}

}