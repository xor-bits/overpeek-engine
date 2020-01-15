#include "form.h"
	
#if _DEBUG && 0
#define _DEFAULT_COLOR (0.0f, 1.0f, 0.0f, 1.0f)
#else
#define _DEFAULT_COLOR (0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG


namespace oe::gui {

	Form::Form(glm::ivec2 size) : 
		Widget::Widget(size),
		m_color(_DEFAULT_COLOR)
	{}

	void Form::render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		renderer.submit(m_render_position, m_size, 0, m_color);
	}

}