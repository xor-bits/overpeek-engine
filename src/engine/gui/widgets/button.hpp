#pragma once

#include "widget.hpp"



namespace oe::gui {

#define BUTTON_CALLBACK_WRAPPER(x) [&](oe::mouse_buttons button, oe::actions action) { x(button, action); }
	typedef std::function<void(oe::mouse_buttons button, oe::actions action)> button_callback;

	struct ButtonInfo {
		button_callback callback  = nullptr;
		glm::ivec2 size           = { 50, 50 };
		glm::vec2 offset_position = { 0, 0 };
		glm::vec2 align_parent    = oe::alignments::center_center;
		glm::vec2 align_render    = oe::alignments::center_center;
	};

	class Button : public Widget {
	public:
		ButtonInfo button_info;

	public:
		Button(const ButtonInfo& button_info);

		// Inherited via Widget
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;
	};

}