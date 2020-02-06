#pragma once

#include "widget.h"



namespace oe::gui {

	class Button : public Widget {
	private:
		typedef std::function<void(oe::mouse_buttons button, oe::actions action)> button_callback;
		
		button_callback m_callback;

	public:
		Button(glm::ivec2 size);

		// Inherited via Widget
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;

		inline void setCallback(button_callback callback) { m_callback = callback; }
	};

}