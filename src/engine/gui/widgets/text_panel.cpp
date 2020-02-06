#include "text_panel.h"

#include "engine/graphics/textLabel.h"



namespace oe::gui {
	
	TextPanel::TextPanel(int font_size) :
		Widget({ font_size, font_size }),
		m_string(""),
		m_bg_color(0.0f, 0.0f, 0.0f, 0.0f),
		m_font_size(font_size)
	{

	}

	void TextPanel::render(oe::graphics::Renderer& renderer) {
		m_size.x = oe::graphics::Text::width(m_string, glm::vec2(m_font_size));
		m_size.y = m_font_size;
		__resize();

		oe::graphics::Text::submit(renderer, m_string, m_render_position + oe::alignmentOffset(m_size, m_align_render), glm::vec2(m_font_size), m_align_render, m_bg_color);
	}

}