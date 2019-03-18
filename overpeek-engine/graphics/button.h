#pragma once

#include <string>
#include <glm/glm.hpp>

namespace oe {

	class Renderer;
	class Button {
	private:
		std::string m_text_a;
		float m_x_a;
		float m_y_a;
		float m_size_x_a;
		float m_size_y_a;
		glm::vec4 m_color_a;

		std::string m_text_b;
		float m_x_b;
		float m_y_b;
		float m_size_x_b;
		float m_size_y_b;
		glm::vec4 m_color_b;

		float m_a_to_b_speed;

		void(*m_callback)(int);

	public:
		Button(float x, float y, float size_x, float size_y, const char* text);
		~Button();

		void render(Renderer *renderer);
		void update(float mouse_x, float mouse_y, int mouse_button, int mouse_action);


		inline float getXA() { return m_x_a; }
		inline float getXA() { return m_y_a; }
		inline float getSizeXA() { return m_size_y_a; }
		inline float getSizeXA() { return m_size_x_a; }
		inline const char *getText() { return m_text.c_str(); }

		inline void setX(float x) { m_x = x; }
		inline void setX(float y) { m_y = y; }
		inline void setSizeX(float x) { m_size_x = x; }
		inline void setSizeX(float y) { m_size_y = y; }
		inline void setText(const char* text) { m_text = text; }
		inline void setCallback(void(*callback)(int)) { m_callback = callback; }

	};

}