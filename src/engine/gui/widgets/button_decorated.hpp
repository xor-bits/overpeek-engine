#pragma once

#include "widget.hpp"
#include "button.hpp"



namespace oe::graphics { struct Sprite; }
namespace oe::gui { class Button; class SpritePanel; class TextPanel; }

namespace oe::gui {

	struct DecoratedButtonInfo {
		glm::ivec2 size                    = { 50, 50 };
		glm::ivec2 padding                 = { 8, 8 };
		glm::ivec2 offset_position         = { 0, 0 };
		std::u32string text                = U"";
		int text_font_size                 = 16;
		std::string text_font_path         = ""; // empty for gui default
		bool autoresize                    = false;
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;
		glm::vec4 color                    = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};
	
	class DecoratedButton : public Widget {
	public:
		DecoratedButtonInfo button_info;
		ButtonHoverEvent event_hover_latest;
		ButtonUseEvent event_use_latest;

	private:
		oe::gui::SpritePanel* button_background;
		oe::gui::TextPanel* button_text;
		oe::gui::Button* button;

	public:
		DecoratedButton(const DecoratedButtonInfo& button_info);
		~DecoratedButton();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;
		
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_button_use(const ButtonUseEvent& e);
		void on_button_hover(const ButtonHoverEvent& e);
	};

}
