#pragma once

#include "widget.h"



namespace oe::gui {

	class TextPanel : public Widget {
	private:
		std::string m_string;
		// baked text label
		glm::vec4 m_bg_color;
		int m_font_size;

	public:
		TextPanel(int font_size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;

		inline std::string& string() { return m_string; };
		inline glm::vec4& bg_color() { return m_bg_color; };
	};

}