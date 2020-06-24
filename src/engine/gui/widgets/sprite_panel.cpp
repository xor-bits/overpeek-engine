#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(_sprite_panel_info.size, _sprite_panel_info.align_parent, _sprite_panel_info.align_render, _sprite_panel_info.offset_position)
		, sprite_panel_info(_sprite_panel_info)
	{}
	
	SpritePanel::~SpritePanel()
	{}

	void SpritePanel::managerAssigned(GUI* gui_manager)
	{
		quad = gui_manager->getRenderer()->createQuad();

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&SpritePanel::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void SpritePanel::managerUnassigned(GUI* gui_manager)
	{
		gui_manager->getRenderer()->destroyQuad(quad);

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&SpritePanel::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void SpritePanel::on_render(const GUIRenderEvent& event)
	{
		if (!toggled) { quad->reset(); return; }

		if (!sprite_panel_info.sprite)
		{
			spdlog::warn("No sprite for SpritePanel");
			return;
		}

		quad->setPosition(render_position);
		quad->setZ(z);
		quad->setSize(size);
		quad->setColor(sprite_panel_info.color);
		quad->setSprite(sprite_panel_info.sprite);
		quad->update();
	}

}