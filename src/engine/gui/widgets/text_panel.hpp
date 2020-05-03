#pragma once

#include "widget.hpp"



namespace oe::graphics { class Quad; class TextLabel; }
namespace oe::gui {

	struct TextPanelInfo {
		std::string text           = "";
		int font_size              = 16;
		glm::vec4 background_color = oe::colors::transparent;
		glm::vec2 offset_position  = { 0, 0 };
		glm::vec2 align_parent     = oe::alignments::center_center;
		glm::vec2 align_render     = oe::alignments::center_center;
	};

	class GUI;
	class TextPanel : public Widget {
	private:
		oe::graphics::TextLabel* label;
		oe::graphics::Quad* quad;
		oe::graphics::Quad* text_quad;

	public:
		TextPanelInfo text_panel_info;

	public:
		TextPanel(GUI* gui_manager, const TextPanelInfo& text_panel_info);
		~TextPanel();

		// Inherited via Widget
		virtual void render(float& z, oe::graphics::Renderer* renderer) override;
	};

}