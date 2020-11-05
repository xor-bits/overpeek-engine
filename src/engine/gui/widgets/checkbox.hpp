#pragma once

#include "widget.hpp"
#include "event.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; struct Sprite; }

namespace oe::gui
{
	class Button;
	struct ButtonHoverEvent;
	struct ButtonUseEvent;
	


	struct CheckboxHoverEvent : BaseHoverEvent {};
	struct CheckboxUseEvent : BaseUseEvent
	{
		bool value;
	};



	class Checkbox : public Widget
	{
	public:
		using value_t = bool;
		using hover_event_t = CheckboxHoverEvent;
		using use_event_t = CheckboxUseEvent;
		struct info_t
		{
			using widget_t = Checkbox;
			
			bool initial                       = false;
			oe::color color_back                   = oe::colors::dark_grey;
			oe::color color_mark                   = oe::colors::lime;
			const oe::graphics::Sprite* sprite = nullptr;

			Widget::Info widget_info             = { { 24, 24 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
		};
	
	private:
		std::unique_ptr<oe::graphics::Quad> quad_check;
		std::unique_ptr<oe::graphics::Quad> quad_box; // hehe
		std::shared_ptr<oe::gui::Button> m_button;

	public:
		info_t m_checkbox_info;
		value_t& m_value;
		CheckboxHoverEvent event_hover_latest;
		CheckboxUseEvent event_use_latest;

	public:
		Checkbox(Widget* parent, GUI& gui_manager, const info_t& checkbox_info, value_t& value_ref);
		Checkbox(Widget* parent, GUI& gui_manager, const info_t& checkbox_info) : Checkbox(parent, gui_manager, checkbox_info, m_checkbox_info.initial) {}
		~Checkbox() override;

		void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
		void on_button_use(const ButtonUseEvent& e);
		void on_button_hover(const ButtonHoverEvent& e);
	};

}