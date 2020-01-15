#pragma once

#include "widget.h"



namespace oe::gui {

	class TextView : public Widget {
	private:
		std::string m_string;
		// baked text label
		glm::vec4 m_bg_color;

	public:
		TextView(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) override;
		virtual void resize() override {}
		virtual void cursor(int button, int action, const glm::vec2& cursor_window) override {}

		inline std::string& string() { return m_string; };
		inline glm::vec4& bg_color() { return m_bg_color; };
	};

}