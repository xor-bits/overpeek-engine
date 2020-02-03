#include "sprite_panel.h"

#if _DEBUG && 0
#define _DEFAULT_COLOR glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)
#else
#define _DEFAULT_COLOR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#endif // _DEBUG

namespace oe::gui {

	SpritePanel::SpritePanel(glm::ivec2 size) :
		Widget::Widget(size),
		m_color(_DEFAULT_COLOR),
		m_sprite(nullptr)
	{}

	void SpritePanel::render(oe::graphics::Renderer& renderer) {
		if (!m_sprite) oe_error_terminate("No sprite for SpritePanel");

		renderer.submit(m_render_position, m_size, m_sprite, m_color);
	}

}