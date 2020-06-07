#pragma once

#include "widget.hpp"



namespace oe::gui {

#define BUTTON_CALLBACK_WRAPPER(x) [&](oe::mouse_buttons button, oe::actions action) { x(button, action); }
	typedef std::function<void(oe::mouse_buttons button, oe::actions action)> fun_button_callback;

	struct ButtonInfo {
		fun_button_callback callback  = nullptr;
		glm::ivec2 size           = { 50, 50 };
		glm::vec2 offset_position = { 0, 0 };
		glm::vec2 align_parent    = oe::alignments::center_center;
		glm::vec2 align_render    = oe::alignments::center_center;
	};

	class Button : public Widget {
	public:
		ButtonInfo button_info;

	public:
		Button(GUI* gui_manager, const ButtonInfo& button_info);
		~Button();

		// events
		void on_button(const MouseButtonEvent& event);
	};

}