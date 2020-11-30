#include "text_panel.hpp"

#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/utility/extra.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui
{
	TextPanel::TextPanel(Widget* parent, GUI& gui_manager, const info_t& _text_panel_info) 
		: Widget(parent, gui_manager, _text_panel_info.widget_info)
		, text_panel_info(_text_panel_info)
	{
		m_info.pixel_size = glm::ivec2(_text_panel_info.text_options.size);
		m_info.fract_size = { 0.0f, 0.0f };
	}
	
	void TextPanel::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			text_quad = m_gui_manager.getRenderer()->create();
			label = new oe::graphics::u32TextLabel(m_gui_manager.getFont(text_panel_info.text_options.font), text_panel_info.text_options.size);

			m_cg_render.connect<GUIRenderEvent, &TextPanel::on_render, TextPanel>(m_gui_manager.m_dispatcher, this);
		}
		else
		{
			m_cg_render.disconnect();

			delete label;
			text_quad.reset();
		}
	}

	void TextPanel::on_render(const GUIRenderEvent& /* event */)
	{
		if(!m_cg_render)
			return;

		label->generate(text_panel_info.text, text_panel_info.text_options);
		m_info.pixel_size = label->getSize();
		m_info.fract_size = { 0.0f, 0.0f };
		Widget::on_pre_render(GUIPreRenderEvent{});

		text_quad->toggle(text_panel_info.text_options.enabled);
		text_quad->setPosition(m_render_position);
		text_quad->setSize(m_render_size);
		text_quad->setZ(m_z);
		text_quad->setSprite(label->getSprite());
		text_quad->setColor(oe::colors::white);
	}

}