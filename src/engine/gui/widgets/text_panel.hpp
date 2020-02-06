#pragma once

#include "widget.h"



namespace oe::gui {

	struct TextPanelInfo {
		std::string text           = "";
		int font_size              = 16;
		glm::vec4 background_color = oe::colors::transparent;
		glm::vec2 offset_position  = { 0, 0 };
		glm::vec2 align_parent     = oe::alignments::center_center;
		glm::vec2 align_render     = oe::alignments::center_center;
	};

	class TextPanel : public Widget {
	public:
		TextPanelInfo text_panel_info;

	public:
		TextPanel(const TextPanelInfo& text_panel_info);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}