#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(GUI* gui_manager, const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(gui_manager, _sprite_panel_info.size, _sprite_panel_info.align_parent, _sprite_panel_info.align_render, _sprite_panel_info.offset_position)
		, sprite_panel_info(_sprite_panel_info)
	{
		quad = m_gui_manager->getRenderer()->createQuad();

		// event listeners
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&SpritePanel::on_render>(this);
	}
	
	SpritePanel::~SpritePanel()
	{
		m_gui_manager->getRenderer()->destroyQuad(quad);

		// event listeners
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&SpritePanel::on_render>(this);
	}

	void SpritePanel::on_render(const GUIRenderEvent& event)
	{
		if (!sprite_panel_info.sprite)
		{
			spdlog::warn("No sprite for SpritePanel");
			return;
		}

		*event.z += 1.0f;
		quad->setPosition(render_position);
		quad->setZ(*event.z);
		quad->setSize(size);
		quad->setColor(sprite_panel_info.color);
		quad->setSprite(sprite_panel_info.sprite);
		quad->update();
	}

}