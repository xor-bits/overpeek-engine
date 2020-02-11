#pragma once

#include "widget.hpp"



namespace oe::gui {

	struct SpritePanelInfo {
		glm::ivec2 size                    = { 50, 50 };
		glm::vec2 offset_position          = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	class SpritePanel : public Widget {
	public:
		SpritePanelInfo sprite_panel_info;

	public:
		SpritePanel(const SpritePanelInfo& sprite_panel_info);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}
