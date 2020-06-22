#pragma once

#include "widget.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui {

	struct SpritePanelInfo {
		glm::ivec2 size                    = { 50, 50 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	struct GUIRenderEvent;
	class GUI;
	class SpritePanel : public Widget {
	private:
		oe::graphics::Quad* quad;
		
	public:
		SpritePanelInfo sprite_panel_info;

	public:
		SpritePanel(const SpritePanelInfo& sprite_panel_info);
		~SpritePanel();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		// events
		void on_render(const GUIRenderEvent& event);
	};

}
