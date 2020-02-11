#include "sprite_panel.hpp"
#include "engine/engine.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(_sprite_panel_info.size, _sprite_panel_info.align_parent, _sprite_panel_info.align_render, _sprite_panel_info.offset_position)
		, sprite_panel_info(_sprite_panel_info)
	{
	
	}

	void SpritePanel::render(oe::graphics::Renderer& renderer) {
		if (!sprite_panel_info.sprite) spdlog::warn("No sprite for SpritePanel");
		renderer.submit(render_position, size, sprite_panel_info.sprite, sprite_panel_info.color);
	}

}