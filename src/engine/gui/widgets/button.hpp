#pragma once

#include "widget.hpp"
#include "engine/enum.hpp"



namespace oe::gui
{
	struct ButtonInfo
	{
		glm::ivec2 size              = { 50, 50 };
		glm::ivec2 offset_position   = { 0, 0 };
		glm::vec2 align_parent       = oe::alignments::center_center;
		glm::vec2 align_render       = oe::alignments::center_center;
	};

	struct ButtonHoverEvent
	{};

	struct ButtonUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
	};

	class Button : public Widget
	{
	public:
		ButtonInfo button_info;
		ButtonHoverEvent event_hover_latest;
		ButtonUseEvent event_use_latest;

	public:
		Button(const ButtonInfo& button_info);
		~Button();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

	private:
		// events
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
	};

}