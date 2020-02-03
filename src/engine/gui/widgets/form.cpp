#include "form.h"

#if _DEBUG && 0
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.2f)
#else
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG


namespace oe::gui {

	Form::Form(glm::ivec2 size) : 
		Widget::Widget(size),
		m_color(_DEFAULT_COLOR)
	{}

	void Form::render(oe::graphics::Renderer& renderer) {
		renderer.submit(m_render_position, m_size, 0, m_color);
	}

}