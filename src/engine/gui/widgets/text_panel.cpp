#include "text_panel.hpp"

#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/renderer.hpp"



namespace oe::gui
{
	TextPanel::TextPanel(const TextPanelInfo& _text_panel_info) 
		: Widget(_text_panel_info.widget_info)
		, text_panel_info(_text_panel_info)
	{}

	TextPanel::~TextPanel()
	{}

	void TextPanel::managerAssigned(GUI* gui_manager)
	{
		text_quad = gui_manager->getLateRenderer()->create();
		label = new oe::graphics::u32TextLabel(gui_manager->getFont(text_panel_info.font_size, text_panel_info.font_path));

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&TextPanel::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void TextPanel::managerUnassigned(GUI* gui_manager)
	{
		text_quad.reset();
		delete label;

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&TextPanel::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void TextPanel::on_render(const GUIRenderEvent& event)
	{
		text_quad->toggle(m_info.toggled);
		if (!m_info.toggled)
		{
			text_quad->update(text_quad);
			return;
		}

		label->generate(text_panel_info.text, m_gui_manager->getWindow(), text_panel_info.background_color);
		m_info.size = label->getSize();
		text_quad->setPosition(static_cast<glm::vec2>(render_position + oe::alignmentOffset(m_info.size, m_info.align_render)));
		text_quad->setZ(z);
		text_quad->setSize(m_info.size);
		text_quad->setSprite(label->getSprite());
		text_quad->setColor(oe::colors::white);
		text_quad->setRotationAlignment(m_info.align_render);
		text_quad->update(text_quad);
	}

}