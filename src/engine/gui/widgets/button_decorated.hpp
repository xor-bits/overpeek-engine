#pragma once

#include "button.h"
#include "text_panel.h"
#include "sprite_panel.h"



namespace oe::gui {

	class DecoratedButton : public Button {
	public:
		DecoratedButton(glm::ivec2 size, const oe::graphics::Sprite* sprite, std::string text);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
	};

}