#pragma once

#include "button.hpp"
#include "text_panel.hpp"
#include "sprite_panel.hpp"



namespace oe::gui {

	class DecoratedButton : public Button {
	public:
		DecoratedButton(glm::ivec2 size, const oe::graphics::Sprite* sprite, std::string text);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}