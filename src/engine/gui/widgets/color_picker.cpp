#include "color_picker.hpp"

#include "engine/engine.hpp"



namespace oe::gui {

	Slider* color_channel(Widget* parent, const glm::vec2& offset, const glm::vec2& size, const std::string& text, const glm::vec4& color, const oe::graphics::Sprite* sprite, std::function<void(float)> update, float initial) {
		oe::gui::SliderInfo slider_info;
		slider_info.slider_size = size;
		slider_info.knob_size = { 25, size.y };
		slider_info.offset_position = offset;
		slider_info.align_parent = oe::alignments::top_left;
		slider_info.align_render = oe::alignments::top_left;
		slider_info.slider_color = color;
		slider_info.slider_sprite = sprite;
		slider_info.knob_sprite = sprite;
		slider_info.callback = update;
		slider_info.initial_value = initial;
		slider_info.min_value = 0.0f;
		slider_info.max_value = 1.0f;
		slider_info.draw_value = true;
		auto slider = new oe::gui::Slider(slider_info);
		parent->addSubWidget(slider);

		return slider;
	}

	ColorPicker::ColorPicker(const ColorPickerInfo& _color_picker_info)
		: Widget(_color_picker_info.size, _color_picker_info.align_parent, _color_picker_info.align_render, _color_picker_info.offset_position)
		, color_picker_info(_color_picker_info)
	{
		if (color_picker_info.callback) color_picker_info.callback(get());

		oe::gui::SpritePanelInfo sprite_panel_info = {};
		sprite_panel_info.size = color_picker_info.size;
		sprite_panel_info.align_parent = color_picker_info.align_parent;
		sprite_panel_info.align_render = color_picker_info.align_render;
		sprite_panel_info.sprite = color_picker_info.sprite;
		sprite_panel_info.color = color_picker_info.background_color;
		auto box = new oe::gui::SpritePanel(sprite_panel_info);
		addSubWidget(box);

		SpritePanelInfo preview_panel_info;
		preview_panel_info.align_parent = oe::alignments::top_right;
		preview_panel_info.align_render = oe::alignments::top_right;
		preview_panel_info.size = { 40, color_picker_info.size.y - 10 };
		preview_panel_info.offset_position = { -5, 5 };
		preview_panel_info.color = color_picker_info.initial_color;
		preview_panel_info.sprite = color_picker_info.sprite;
		preview_panel = new SpritePanel(preview_panel_info);
		addSubWidget(preview_panel);

		int slider_height = static_cast<int>((color_picker_info.size.y - 25) / 4.0f);
		int slider_width = color_picker_info.size.x - 55;

		auto red =   color_channel(this, { 5, 0 * slider_height + 5  }, { slider_width, slider_height }, "R", oe::colors::red, color_picker_info.sprite, [&](float value) { color_picker_info.initial_color.r = value; update(); }, color_picker_info.initial_color.r);
		auto green = color_channel(this, { 5, 1 * slider_height + 10 }, { slider_width, slider_height }, "G", oe::colors::green, color_picker_info.sprite, [&](float value) { color_picker_info.initial_color.g = value; update(); }, color_picker_info.initial_color.g);
		auto blue =  color_channel(this, { 5, 2 * slider_height + 15 }, { slider_width, slider_height }, "B", oe::colors::blue, color_picker_info.sprite, [&](float value) { color_picker_info.initial_color.b = value; update(); }, color_picker_info.initial_color.b);
		auto alpha = color_channel(this, { 5, 3 * slider_height + 20 }, { slider_width, slider_height }, "A", oe::colors::black, color_picker_info.sprite, [&](float value) { color_picker_info.initial_color.a = value; update(); }, color_picker_info.initial_color.a);
		
	}

}