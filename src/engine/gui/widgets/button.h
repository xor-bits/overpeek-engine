#pragma once

#include "text_view.h"



namespace oe::gui {

	class Button : public TextView {
	private:
		glm::vec4 m_color;
		void(*m_callback)();

	public:
		Button(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) override;
		virtual void resize() override;
		virtual void cursor(int button, int action, int x, int y) override;

		inline glm::vec4& color() { return m_color; }
		inline void setCallback(void(*callback)()) { m_callback = callback; }
	};

}