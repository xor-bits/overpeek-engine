#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(Widget* parent, GUI& gui_manager, const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(parent, gui_manager, _sprite_panel_info.widget_info)
		, sprite_panel_info(_sprite_panel_info)
	{
	}
	
	SpritePanel::~SpritePanel()
	{}
	
	void SpritePanel::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			if (sprite_panel_info.sprite_alpha)
				quad = m_gui_manager.getLateRenderer()->create();
			else
				quad = m_gui_manager.getRenderer()->create();

			m_cg_render = { m_gui_manager.dispatcher, this };
		}
		else
		{
			m_cg_render.reset();
			
			quad.reset();
		}
	}

	void SpritePanel::on_render(const GUIRenderEvent& event)
	{
		quad->setPosition(render_position + oe::alignmentOffset(m_info.size, oe::alignments::center_center));
		quad->setZ(z);
		quad->setSize(m_info.size);
		quad->setColor(sprite_panel_info.color);
		quad->setSprite(sprite_panel_info.sprite);
		quad->setRotation(sprite_panel_info.rotation);
		quad->setRotationAlignment(oe::alignments::center_center);
		quad->update();
	}

}