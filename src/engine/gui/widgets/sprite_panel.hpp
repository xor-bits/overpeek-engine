#pragma once

#include "widget.hpp"
#include "engine/graphics/primitives.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; struct Sprite; }

namespace oe::gui
{
	struct SpritePanelInfo
	{
		glm::vec4 color                    = oe::colors::white;
		const oe::graphics::Sprite* sprite = nullptr;
		float rotation                     = 0.0f;
		
		WidgetInfo widget_info             = { { 50, 50 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};
	
	class SpritePanel : public Widget
	{
	private:
		std::unique_ptr<oe::graphics::Quad> quad;
		
	public:
		SpritePanelInfo sprite_panel_info;

	public:
		SpritePanel(Widget* parent, GUI& gui_manager, const SpritePanelInfo& sprite_panel_info = {});

		virtual void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
	};

}
