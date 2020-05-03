#pragma once

#include "widget.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui {

	struct SpritePanelInfo {
		glm::ivec2 size                    = { 50, 50 };
		glm::vec2 offset_position          = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	class GUI;
	class SpritePanel : public Widget {
	private:
		oe::graphics::Quad* quad;
		
	public:
		SpritePanelInfo sprite_panel_info;

	public:
		SpritePanel(GUI* gui_manager, const SpritePanelInfo& sprite_panel_info);
		~SpritePanel();

		// Inherited via Widget
		virtual void render(float& z, oe::graphics::Renderer* renderer) override;
	};

}
