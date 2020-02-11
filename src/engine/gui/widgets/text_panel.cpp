#include "text_panel.hpp"

#include "engine/graphics/textLabel.hpp"



namespace oe::gui {
	
	TextPanel::TextPanel(const TextPanelInfo& _text_panel_info) 
		: Widget(glm::vec2(_text_panel_info.font_size), _text_panel_info.align_parent, _text_panel_info.align_render, _text_panel_info.offset_position)
		, text_panel_info(_text_panel_info)
	{

	}

	void TextPanel::render(oe::graphics::Renderer& renderer) {
		oe::graphics::Text::submit(renderer, text_panel_info.text, render_position + oe::alignmentOffset(size, align_render), glm::vec2(text_panel_info.font_size), align_render, text_panel_info.background_color);
	}

}