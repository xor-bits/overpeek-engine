#include "text_view.h"

#include <engine/graphics/text/textLabel.h>



namespace oe::gui {
	
	TextView::TextView(glm::ivec2 size) :
		Widget(size),
		m_string(""),
		m_bg_color(0.0f, 0.0f, 0.0f, 0.0f)
	{

	}

	void TextView::render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		oe::graphics::Text::submit(text_renderer, m_string, m_render_position + oe::graphics::alignmentOffset(m_size, m_align_render), m_size, m_align_render, m_bg_color);
	}

}