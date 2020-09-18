#include "text_panel.hpp"

#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/utility/extra.hpp"



namespace oe::gui
{
	TextPanel::TextPanel(Widget* parent, GUI& gui_manager, const TextPanelInfo& _text_panel_info) 
		: Widget(parent, gui_manager, _text_panel_info.widget_info)
		, text_panel_info(_text_panel_info)
	{}

	TextPanel::~TextPanel()
	{}
	
	void TextPanel::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			text_quad = m_gui_manager.getLateRenderer()->create();
			label = new oe::graphics::u32TextLabel(m_gui_manager.getFont(text_panel_info.font_size, text_panel_info.font_path));

			m_cg_render = { m_gui_manager.dispatcher, this };
		}
		else
		{
			m_cg_render.reset();

			delete label;
			text_quad.reset();
		}
	}

	void TextPanel::on_render(const GUIRenderEvent& event)
	{
		label->generate(text_panel_info.text, m_gui_manager.getWindow(), text_panel_info.background_color);
		m_info.size = label->getSize();
		Widget::on_pre_render(GUIPreRenderEvent{});

		text_quad->setPosition(render_position);
		text_quad->setZ(z);
		text_quad->setSize(m_info.size);
		text_quad->setSprite(label->getSprite());
		text_quad->setColor(oe::colors::white);
		text_quad->setRotationAlignment(m_info.align_render);
		text_quad->update();
	}

}