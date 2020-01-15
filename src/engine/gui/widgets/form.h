#pragma once

#include "widget.h"



namespace oe::gui {

	class Form : public Widget {
	private:
		glm::vec4 m_color;

	public:
		Form(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) override;
		virtual void resize() override;
		virtual void cursor(int button, int action, int x, int y) override;

		inline glm::vec4& color() { return m_color; }
	};

}