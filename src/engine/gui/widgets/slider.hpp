#pragma once

#include "widget.h"



namespace oe::gui {

	class Slider : public Widget {
	private:
		void(*m_callback)(float val);
		float m_min;
		float m_max;
		float m_value;
		bool m_dragging;
		glm::vec4 m_slider_color;
		glm::vec4 m_bg_color;
		glm::vec2 m_slider_size;

		const oe::graphics::Sprite* m_sprite;

	public:
		Slider(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;

		inline void setCallback(void(*callback)(float value)) { m_callback = callback; }
		inline glm::vec4& sliderColor() { return m_slider_color; }
		inline glm::vec4& bgColor() { return m_bg_color; }
		inline glm::vec2& sliderSize() { return m_slider_size; }
		inline void setSprite(const oe::graphics::Sprite* sprite) { m_sprite = sprite; }
		inline float& min() { return m_min; }
		inline float& max() { return m_max; }
	};

}