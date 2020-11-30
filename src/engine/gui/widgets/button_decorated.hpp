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
			
			// visuals
			oe::utils::color_string<char32_t> text = {{ U"Button", oe::colors::white }};
			TextOptions               text_options = {};
			oe::color                    btn_color = colors::dark_grey;
			const oe::graphics::Sprite*     sprite = nullptr;
			// base
			Button::info_t             button_info = {};
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
	};

}
