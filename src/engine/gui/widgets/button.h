#pragma once

#include "widget.h"



namespace oe::gui {

	class Button : public Widget {
	private:
		void(*m_callback)(int button, int action);

	public:
		Button(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override {}
		virtual void resize() override {}
		virtual void cursor(int button, int action, const glm::vec2& cursor_window) override;
		virtual void text(unsigned int character, int mods) override {}

		inline void setCallback(void(*callback)(int button, int action)) { m_callback = callback; }
	};

}