#pragma once

#include "widget.hpp"



namespace oe::graphics { struct Quad; struct Sprite; }

namespace oe::gui {

	struct SpritePanelInfo {
		glm::ivec2 size                    = { 50, 50 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};
	
	class SpritePanel : public Widget {
	private:
		std::shared_ptr<oe::graphics::Quad> quad;
		
	public:
		SpritePanelInfo sprite_panel_info;

	public:
		SpritePanel(const SpritePanelInfo& sprite_panel_info);
		~SpritePanel();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

	private:
		// events
		void on_render(const GUIRenderEvent& event);
	};

}
