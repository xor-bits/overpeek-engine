#pragma once

#include "widgets/widget.h"
#include "widgets/form.h"
#include "widgets/text_view.h"
#include "widgets/button.h"



namespace oe::gui {

	class GUI {
	public:
		GUI();
		~GUI();
		
		// needs to be after every window resize
		void resize(const glm::vec2& window_size);

		// calls resize(window width, window height)
		void resize();

		void cursor(int button, int action, const glm::vec2& cursor_window);
		
		// assigns std::unique_ptr for this pointer
		// do forget pointer to this widget
		void addSubWidget(Widget* widget);
		
		void render();
		
	};

}