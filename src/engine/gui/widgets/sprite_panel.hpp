#pragma once

#include "widget.hpp"
#include "engine/graphics/sprite.hpp"



namespace oe::gui {

	class SpritePanel : public Widget {
	private:
		glm::vec4 m_color;
		const oe::graphics::Sprite* m_sprite;

	public:
		SpritePanel(glm::ivec2 size);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;

		inline glm::vec4& color() { return m_color; }
		inline void setSprite(const oe::graphics::Sprite* sprite) { m_sprite = sprite; }
	};

}