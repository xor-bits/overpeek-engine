#pragma once

#include "widget.hpp"
#include "button.hpp"



namespace oe::graphics { struct Sprite; }
namespace oe::gui { class Button; class SpritePanel; class TextPanel; }

namespace oe::gui
{
	struct DecoratedButtonInfo
	{
		glm::ivec2 padding                 = { 8, 8 };
		std::u32string text                = U"";
		size_t text_font_size              = 28;
		std::string text_font_path         = ""; // empty for gui default
		bool autoresize                    = false;
		glm::vec4 color                    = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr;

		ButtonInfo button_info;
	};
	
	class DecoratedButton : public Button
	{
	public:
		DecoratedButtonInfo button_info;
		ButtonHoverEvent event_hover_latest;
		ButtonUseEvent event_use_latest;

	private:
		SpritePanel* button_background;
		TextPanel* button_text;

	public:
		DecoratedButton(const DecoratedButtonInfo& button_info = {});
		~DecoratedButton();

		virtual void managerAssigned() override;
		virtual void managerUnassigned() override;
		
	private:
		// events
		void on_render(const GUIRenderEvent& event);
	};

}
