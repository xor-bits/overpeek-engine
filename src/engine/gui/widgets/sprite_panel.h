#pragma once

#include "widget.h"
#include "engine/graphics/sprite.h"



namespace oe::gui {

	class SpritePanel : public Widget {
	private:
		glm::vec4 m_color;
		const oe::graphics::Sprite* m_sprite;

	public:
		SpritePanel(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void resize() override {}
		virtual void cursor(int button, int action, const glm::vec2& cursor_window) override {}
		virtual void text(unsigned int character, int mods) override {}

		inline glm::vec4& color() { return m_color; }
		inline void setSprite(const oe::graphics::Sprite* sprite) { m_sprite = sprite; }
	};

}