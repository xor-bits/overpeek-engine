#pragma once

#include "widget.hpp"
#include "button.hpp"
#include "engine/enum.hpp"
#include "engine/utility/color_string.hpp"



namespace oe::graphics { struct Sprite; }
namespace oe::gui { class SpritePanel; class TextPanel; }

namespace oe::gui
{
	struct DecoratedButtonHoverEvent : ButtonHoverEvent {};
	struct DecoratedButtonUseEvent : ButtonUseEvent {};



	class DecoratedButton : public Button
	{
	public:
		using hover_event_t = DecoratedButtonHoverEvent;
		using use_event_t = DecoratedButtonUseEvent;
		struct info_t
		{
			using widget_t = DecoratedButton;

			oe::utils::color_string<char32_t> text             = {{ U"Button", oe::colors::white }};
			uint16_t text_font_size                            = 28;
			std::string text_font_path                         = ""; // empty for gui default
			
			glm::ivec2 padding                                 = { 8, 8 };
			bool autoresize                                    = false;
			oe::color btn_color                                = colors::dark_grey;
			const oe::graphics::Sprite* sprite                 = nullptr;

			Button::info_t button_info                         = {};
		};
	
	public:
		info_t button_info;
		DecoratedButtonHoverEvent event_hover_latest;
		DecoratedButtonUseEvent event_use_latest;

	private:
		std::shared_ptr<SpritePanel> button_background;
		std::shared_ptr<TextPanel> button_text;

	public:
		DecoratedButton(Widget* parent, GUI& gui_manager, const info_t& button_info);
		~DecoratedButton() override = default;

		void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
	};

}
