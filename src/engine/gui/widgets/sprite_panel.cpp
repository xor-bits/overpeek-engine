#include "sprite_panel.h"
#include "engine/engine.h"

#if _DEBUG && 0
#define _DEFAULT_COLOR glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)
#else
#define _DEFAULT_COLOR glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#endif // _DEBUG

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