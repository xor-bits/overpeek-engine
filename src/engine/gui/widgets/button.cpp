#include "button.h"

#if _DEBUG
#define _DEFAULT_COLOR (0.0f, 1.0f, 0.0f, 1.0f)
#else
#define _DEFAULT_COLOR (0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG


namespace oe::gui {

	Button::Button(glm::ivec2 size) :
		TextView::TextView(size),
		m_color(_DEFAULT_COLOR),
		m_callback(nullptr)
	{

	}

	void Button::render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		renderer.submit(m_render_position, m_size, 0, m_color);

		TextView::render(renderer, text_renderer);
	}

	void Button::resize() {}

	void Button::cursor(int button, int action, int x, int y) {
		if (x >= m_render_position.x && x < m_render_position.x + m_size.x && y >= m_render_position.y && y < m_render_position.y + m_size.y) {
			if (m_callback) m_callback();
		}
	}

}