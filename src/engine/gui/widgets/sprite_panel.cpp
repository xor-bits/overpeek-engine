#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui {

	SpritePanel::SpritePanel(Widget* parent, GUI& gui_manager, const SpritePanelInfo& _sprite_panel_info)
		: Widget::Widget(parent, gui_manager, _sprite_panel_info.widget_info)
		, sprite_panel_info(_sprite_panel_info)
	{
	}
	
	void SpritePanel::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			quad = m_gui_manager.getRenderer()->create();

			m_cg_render.connect<GUIRenderEvent, &SpritePanel::on_render, SpritePanel>(m_gui_manager.dispatcher, this);
		}
		else
		{
			m_cg_render.disconnect();
			
			quad.reset();
		}
	}

	void SpritePanel::on_render(const GUIRenderEvent& /* event */)
	{
		if(!m_cg_render)
			return;

		quad->setPosition(render_position + oe::alignmentOffset(m_info.size, oe::alignments::center_center));
		quad->setZ(z);
		quad->setSize(m_info.size);
		quad->setColor(sprite_panel_info.color);
		quad->setSprite(sprite_panel_info.sprite);
		quad->setRotation(sprite_panel_info.rotation);
		quad->setRotationAlignment(oe::alignments::center_center);
	}

}