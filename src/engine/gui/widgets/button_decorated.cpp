#include "button_decorated.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/connect_guard_additions.hpp"

#include "text_panel.hpp"
#include "sprite_panel.hpp"
#include "button.hpp"



namespace oe::gui
{
	DecoratedButton::DecoratedButton(Widget* parent, GUI& gui_manager, const info_t& _button_info)
		: Button(parent, gui_manager, _button_info.button_info)
		, button_info(_button_info)
	{
		SpritePanel::info_t sp_info;
		sp_info.widget_info.pixel_size = { 0, 0 };
		sp_info.widget_info.fract_size = { 1.0f, 1.0f };
		sp_info.widget_info.pixel_origon_offset = { 0, 0 };
		sp_info.widget_info.fract_origon_offset = oe::alignments::top_left;
		sp_info.widget_info.align_origon = oe::alignments::top_left;
		sp_info.sprite = button_info.sprite;
		sp_info.color_tint = button_info.btn_color;
		button_background = create(sp_info);

		TextPanel::info_t tp_info;
		tp_info.text = button_info.text;
		tp_info.text_options = _button_info.text_options;
		tp_info.widget_info.fract_origon_offset = _button_info.text_options.align;
		tp_info.widget_info.align_origon = _button_info.text_options.align;
		button_text = create(tp_info);
	}
}