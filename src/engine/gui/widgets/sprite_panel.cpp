#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(_sprite_panel_info.widget_info)
		, sprite_panel_info(_sprite_panel_info)
	{
	}
	
	SpritePanel::~SpritePanel()
	{}

	void SpritePanel::managerAssigned(GUI* gui_manager)
	{
		if (sprite_panel_info.sprite_alpha)
			quad = gui_manager->getLateRenderer()->create();
		else
			quad = gui_manager->getRenderer()->create();

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&SpritePanel::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void SpritePanel::managerUnassigned(GUI* gui_manager)
	{
		quad.reset();

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&SpritePanel::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void SpritePanel::on_render(const GUIRenderEvent& event)
	{
		quad->toggle(m_info.toggled);
		if (!m_info.toggled)
		{
			quad->update(quad);
			return;
		}

		NULL_SPRITE_CHECK(sprite_panel_info.sprite);

		quad->setPosition(render_position);
		quad->setZ(z);
		quad->setSize(m_info.size);
		quad->setColor(sprite_panel_info.color);
		quad->setSprite(sprite_panel_info.sprite);
		quad->update(quad);
	}

}