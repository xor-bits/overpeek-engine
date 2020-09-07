#pragma once

#include "widget.hpp"



namespace oe::graphics { class Quad; struct Sprite; }

namespace oe::gui
{
	struct SpritePanelInfo
	{
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
		float rotation                     = 0.0f;
		bool sprite_alpha                  = false;
		
		WidgetInfo widget_info             = { { 50, 50 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};
	
	class SpritePanel : public Widget
	{
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
