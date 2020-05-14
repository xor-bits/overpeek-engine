#include "text_panel.hpp"

#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {
	
	TextPanel::TextPanel(GUI* gui_manager, const TextPanelInfo& _text_panel_info) 
		: Widget(gui_manager, glm::vec2(static_cast<float>(_text_panel_info.font_size)), _text_panel_info.align_parent, _text_panel_info.align_render, _text_panel_info.offset_position)
		, text_panel_info(_text_panel_info)
	{
		label = new oe::graphics::TextLabel();
		text_quad = m_gui_manager->getLateRenderer()->createQuad();
	}

	TextPanel::~TextPanel()
	{
		m_gui_manager->getLateRenderer()->destroyQuad(text_quad);
		delete label;
	}

	void TextPanel::render(float& z, oe::graphics::Renderer* renderer) {
		glm::vec2 text_size = glm::vec2(static_cast<float>(text_panel_info.font_size));
		label->generate(text_panel_info.text, m_gui_manager->getWindow(), text_panel_info.background_color);
		size = text_size * label->getSize();
		text_quad->setPosition(render_position + oe::alignmentOffset(size, align_render));
		text_quad->setZ(z);
		text_quad->setSize(size);
		text_quad->setSprite(label->getSprite());
		text_quad->setColor(oe::colors::white);
		text_quad->setRotationAlignment(align_render);
		text_quad->update();

	}

}