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
		float m_t;

		void(*m_callback)(int);

	public:
		Button(float x, float y, float size_x, float size_y, const char* text);
		Button(float x_a, float y_a, float size_x_a, float size_y_a, const char* text_a, glm::vec4 color_a, float x_b, float y_b, float size_x_b, float size_y_b, const char* text_b, glm::vec4 color_b, float transform_speed);
		~Button();

		void render(Renderer *renderer, float corrector);
		void update(float mouse_x, float mouse_y, int mouse_button, int mouse_action, int ups);


		inline float getXA() { return m_x_a; }
		inline float getXA() { return m_y_a; }
		inline float getSizeXA() { return m_size_y_a; }
		inline float getSizeXA() { return m_size_x_a; }
		inline const char *getTextA() { return m_text_a.c_str(); }
		inline const char *getTextB() { return m_text_b.c_str(); }
		inline glm::vec4 *getColorA() { return &m_color_a; }
		inline glm::vec4 *getColorB() { return &m_color_b; }

		inline void setXA(float val) { m_x_a = val; }
		inline void setXA(float val) { m_y_a = val; }
		inline void setSizeXA(float val) { m_size_y_a = val; }
		inline void setSizeXA(float val) { m_size_x_a = val; }
		inline void setTextA(const char *text) { m_text_a = text; }
		inline void setTextB(const char *text) { m_text_b = text; }
		inline void setColorA(glm::vec4 *val) { m_color_a = *val; }
		inline void setColorB(glm::vec4 *val) { m_color_b = *val; }

		inline void setCallback(void(*callback)(int)) { m_callback = callback; }

	};

}