#include "button.h"

namespace oe {

	Button::Button(float x, float y, float size_x, float size_y, const char* text)
	: m_x(x), m_y(y), m_size_x(size_x), m_size_y(size_y), m_text(text) {

	}
	
	Button::~Button() {

	}

	void Button::render(Renderer *renderer) {

	}

	void Button::update(float mouse_x, float mouse_y, int mouse_button, int mouse_action) {

	}

}