#include "button_decorated.hpp"
#include "engine/gui/gui_manager.hpp"

#if _DEBUG && 0
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.2f)
#else
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG



namespace oe::gui {

	DecoratedButton::DecoratedButton(const DecoratedButtonInfo& _button_info)
		: Widget(_button_info.size, _button_info.align_parent, _button_info.align_render, _button_info.offset_position)
		, button_info(_button_info)
	{
		ButtonInfo b_info = {};
		b_info.size = button_info.size;
		b_info.align_parent = oe::alignments::center_center;
		b_info.align_render = oe::alignments::center_center;
		b_info.callback = button_info.callback;
		button = new Button(b_info);
		addSubWidget(button);

		SpritePanelInfo sp_info = {};
		sp_info.size = button_info.size;
		sp_info.align_parent = oe::alignments::center_center;
		sp_info.align_render = oe::alignments::center_center;
		sp_info.sprite = button_info.sprite;
		sp_info.color = button_info.color;
		button_background = new oe::gui::SpritePanel(sp_info);
		button->addSubWidget(button_background);

		TextPanelInfo tp_info = {};
		tp_info.font_size = static_cast<int>(static_cast<float>(button_info.size.y) * 0.8f);
		tp_info.text = button_info.text;
		tp_info.align_parent = oe::alignments::center_center;
		tp_info.align_render = oe::alignments::center_center;
		button_text = new oe::gui::TextPanel(tp_info);
		button_background->addSubWidget(button_text);
	}

	DecoratedButton::~DecoratedButton() {
	}

	void DecoratedButton::managerAssigned(GUI* gui_manager)
	{
		
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&DecoratedButton::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void DecoratedButton::managerUnassigned(GUI* gui_manager)
	{
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&DecoratedButton::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void DecoratedButton::on_render(const GUIRenderEvent& event)
	{
		if (!button_info.autoresize) return;

		const glm::ivec2 new_size = button_text->size + button_info.padding * 2;
		button_background->size = new_size;
		button->size = new_size;
		size = new_size;
	}

}