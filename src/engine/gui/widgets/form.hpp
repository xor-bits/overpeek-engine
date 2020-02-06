#pragma once

#include "widget.hpp"



namespace oe::gui {

	class Form : public Widget {
	private:
		glm::vec4 m_color;

	public:
		Form(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;

		inline glm::vec4& color() { return m_color; }
	};

}