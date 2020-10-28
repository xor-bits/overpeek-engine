#pragma once

#include "widget.hpp"
#include "event.hpp"
#include "engine/enum.hpp"



namespace oe::gui
{
	struct ButtonHoverEvent : BaseHoverEvent {};
	struct ButtonUseEvent : BaseUseEvent {};


	
	class Button : public Widget
	{
	public:
		struct info_t : Widget::info_t {
			using widget_t = Button;
		};

	public:
		info_t button_info;
		ButtonHoverEvent event_hover_latest;
		ButtonUseEvent event_use_latest;

	public:
		Button(Widget* parent, GUI& gui_manager, const info_t& button_info);
		~Button() override = default;

		bool test(const glm::vec2& point);

		void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		oe::utils::connect_guard m_cg_cursor;
		oe::utils::connect_guard m_cg_button;
	};

}