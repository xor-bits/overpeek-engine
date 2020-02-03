#include "button_decorated.h"

#if _DEBUG && 0
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.2f)
#else
#define _DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
#endif // _DEBUG


namespace oe::gui {

	DecoratedButton::DecoratedButton(glm::ivec2 size, const oe::graphics::Sprite* sprite, std::string text) :
		Button::Button(size)
	{
		auto button_background = new oe::gui::SpritePanel(size);
		button_background->align_parent() = oe::graphics::alignment::center_center;
		button_background->align_render() = oe::graphics::alignment::center_center;
		button_background->setSprite(sprite);
		button_background->color() = oe::colors::grey;
		addSubWidget(button_background);

		auto button_text = new oe::gui::TextPanel(size.y * 0.8f);
		button_text->align_parent() = oe::graphics::alignment::center_center;
		button_text->align_render() = oe::graphics::alignment::center_center;
		button_text->string() = text;
		button_background->addSubWidget(button_text);
	}

	void DecoratedButton::render(oe::graphics::Renderer& renderer) {
		Button::render(renderer);
	}

}