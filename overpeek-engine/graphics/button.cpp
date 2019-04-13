#include "button.h"

namespace oe {

	Button::Button(float x, float y, float size_x, float size_y, const char* text)
	: Button(x, y, size_x, size_y, text, glm::vec4(1.0f), x, y, size_x, size_y, text, glm::vec4(1.0f), 0.0f) {

	}

	Button::Button(float x_a, float y_a, float size_x_a, float size_y_a, const char* text_a, glm::vec4 color_a, float x_b, float y_b, float size_x_b, float size_y_b, const char* text_b, glm::vec4 color_b, float transform_speed) 
	: m_x_a(x_a), m_y_a(y_a), m_size_x_a(size_x_a), m_size_y_a(size_y_a), m_text_a(text_a), m_color_a(color_a),
		m_x_b(x_b), m_y_b(y_b), m_size_x_b(size_x_b), m_size_y_b(size_y_b), m_text_b(text_b), m_color_b(color_b), m_a_to_b_speed(transform_speed), m_t(0.0f) {

	}
	
	Button::~Button() {

	}

	void Button::render(Renderer *renderer, float corrector) {

	}

	void Button::update(float mouse_x, float mouse_y, int mouse_button, int mouse_action, int ups) {

	}

}