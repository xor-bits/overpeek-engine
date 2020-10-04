#pragma once

#include "widget.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; struct Sprite; }
namespace oe::gui { class Button; struct ButtonUseEvent; struct ButtonHoverEvent; }

namespace oe::gui
{
	struct CheckboxInfo
	{
		bool initial                       = false;
		glm::vec4 color_back               = oe::colors::dark_grey;
		glm::vec4 color_mark               = oe::colors::lime;
		const oe::graphics::Sprite* sprite = nullptr;

		WidgetInfo widget_info             = { { 24, 24 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct CheckboxHoverEvent
	{};

	struct CheckboxUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		bool value;
	};

	class Checkbox : public Widget
	{
	private:
		std::unique_ptr<oe::graphics::Quad> quad_check;
		std::unique_ptr<oe::graphics::Quad> quad_box; // hehe
		std::shared_ptr<oe::gui::Button> m_button;

	public:
		CheckboxInfo m_checkbox_info;
		bool& m_value;
		CheckboxHoverEvent event_hover_latest;
		CheckboxUseEvent event_use_latest;

	public:
		Checkbox(Widget* parent, GUI& gui_manager, bool& value_ref, const CheckboxInfo& checkbox_info = {});
		Checkbox(Widget* parent, GUI& gui_manager, const CheckboxInfo& checkbox_info = {}) : Checkbox(parent, gui_manager, m_checkbox_info.initial, checkbox_info) {}
		~Checkbox();

		virtual void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
		void on_button_use(const ButtonUseEvent& e);
		void on_button_hover(const ButtonHoverEvent& e);
	};

}