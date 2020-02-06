#include "form.h"



namespace oe::gui {

	Form::Form(glm::ivec2 size) : 
		Widget::Widget(size),
		m_color(0.0f, 0.0f, 0.0f, 0.2f)
	{}

	void Form::render(oe::graphics::Renderer& renderer) {
#if _DEBUG
		renderer.submit(m_render_position, m_size, 0, m_color);
#endif // _DEBUG
	}

}