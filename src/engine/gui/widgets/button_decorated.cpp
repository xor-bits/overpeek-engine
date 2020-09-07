#include "button_decorated.hpp"
#include "engine/gui/gui_manager.hpp"

#include "text_panel.hpp"
#include "sprite_panel.hpp"
#include "button.hpp"



namespace oe::gui {

	DecoratedButton::DecoratedButton(const DecoratedButtonInfo& _button_info)
		: Button(_button_info.button_info)
		, button_info(_button_info)
	{
		SpritePanelInfo sp_info;
		sp_info.widget_info.size = m_info.size;
		sp_info.widget_info.align_parent = oe::alignments::center_center;
		sp_info.widget_info.align_render = oe::alignments::center_center;
		sp_info.sprite = button_info.sprite;
		sp_info.color = button_info.color;
		button_background = new oe::gui::SpritePanel(sp_info);
		addSubWidget(button_background);

		TextPanelInfo tp_info;
		tp_info.text = button_info.text;
		tp_info.font_size = _button_info.text_font_size;
		tp_info.widget_info.align_parent = oe::alignments::center_center;
		tp_info.widget_info.align_render = oe::alignments::center_center;
		button_text = new oe::gui::TextPanel(tp_info);
		addSubWidget(button_text);
	}

	DecoratedButton::~DecoratedButton()
	{
	}

	void DecoratedButton::managerAssigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&DecoratedButton::on_render>(this);

		Button::managerAssigned(gui_manager);
	}

	void DecoratedButton::managerUnassigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&DecoratedButton::on_render>(this);

		Button::managerUnassigned(gui_manager);
	}

	void DecoratedButton::on_render(const GUIRenderEvent& event)
	{
		if (!button_info.autoresize) return;

		const glm::ivec2 new_size = button_text->m_info.size + button_info.padding * 2;
		button_background->m_info.size = new_size;
		m_info.size = new_size;
	}

}