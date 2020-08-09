#include "color_picker.hpp"

#include "slider.hpp"
#include "sprite_panel.hpp"
#include "engine/engine.hpp"



namespace oe::gui
{
	VecSliderInfo<4> create_info(const ColorPickerInfo& color_picker_info)
	{
		const int slider_height = color_picker_info.size.y;
		const int slider_width = color_picker_info.size.x - 55;
		const float mult = color_picker_info.draw_value == 2 ? 256.0f : 1.0f;

		VecSliderInfo<4> info;
		info.type = arrangements::rows;
		info.initial_values = color_picker_info.initial_color * mult;
		info.min_values = glm::vec4(0.0f);
		info.max_values = glm::vec4(mult);
		
		info.slider_size = { slider_width - 10, slider_height - 10 };
		info.offset_position = color_picker_info.offset_position + glm::ivec2{ 5, 5 };
		info.align_parent = color_picker_info.align_parent;
		info.align_render = color_picker_info.align_render;
		info.draw_value = color_picker_info.draw_value != 0;
		info.slider_sprite = color_picker_info.sprite;
		info.knob_sprite = color_picker_info.sprite;
		info.draw_value = true;
		info.slider_borders = 5;

		return info;
	}

	ColorPicker::ColorPicker(const ColorPickerInfo& _color_picker_info)
		: VecSlider<4>(create_info(_color_picker_info))
		, color_picker_info(_color_picker_info)
		, mult(color_picker_info.draw_value == 2 ? 256.0f : 1.0f)
	{
		color_picker_info.initial_color *= mult;
		color_picker_info.size = VecSlider<4>::size;

		SpritePanelInfo sprite_panel_info = {};
		sprite_panel_info.size = color_picker_info.size + glm::ivec2{ 55, 10 };
		sprite_panel_info.align_parent = oe::alignments::top_left;
		sprite_panel_info.align_render = oe::alignments::top_left;
		sprite_panel_info.offset_position = { -5, -5 };
		sprite_panel_info.sprite = color_picker_info.sprite;
		sprite_panel_info.color = color_picker_info.background_color;
		auto bgbox = new SpritePanel(sprite_panel_info);
		bgbox->overrideZ(bgbox->getZ() - 0.5f);
		addSubWidget(bgbox);

		SpritePanelInfo preview_panel_info;
		preview_panel_info.align_parent = oe::alignments::top_right;
		preview_panel_info.align_render = oe::alignments::top_right;
		preview_panel_info.size = { 40, color_picker_info.size.y };
		preview_panel_info.offset_position = { -5, 5 };
		preview_panel_info.color = color_picker_info.initial_color;
		preview_panel_info.sprite = color_picker_info.sprite;
		preview_panel = new SpritePanel(preview_panel_info);
		bgbox->addSubWidget(preview_panel);

		sliders[0]->slider_info.slider_lcolor = oe::colors::red;
		sliders[1]->slider_info.slider_lcolor = oe::colors::green;
		sliders[2]->slider_info.slider_lcolor = oe::colors::blue;
		sliders[3]->slider_info.slider_lcolor = oe::colors::black;


		connect_listener<VecSliderHoverEvent<4>, &ColorPicker::on_vec_slider_hover>(this);
		connect_listener<VecSliderUseEvent<4>, &ColorPicker::on_vec_slider_use>(this);
	}

	ColorPicker::~ColorPicker()
	{
		disconnect_listener<VecSliderHoverEvent<4>, &ColorPicker::on_vec_slider_hover>(this);
		disconnect_listener<VecSliderUseEvent<4>, &ColorPicker::on_vec_slider_use>(this);
	}

	void ColorPicker::on_vec_slider_hover(const VecSliderHoverEvent<4>& e)
	{
		dispatcher.trigger(event_hover_latest);
	}

	void ColorPicker::on_vec_slider_use(const VecSliderUseEvent<4>& e)
	{
		color_picker_info.initial_color = e.value / mult;
		update();

		event_use_latest.action = e.action;
		event_use_latest.button = e.button;
		event_use_latest.modifier = e.modifier;
		event_use_latest.value = color_picker_info.initial_color;
		dispatcher.trigger(event_use_latest);
	}
	
	const glm::vec4& ColorPicker::get() const
	{
		return color_picker_info.initial_color;
	}

	void ColorPicker::set(const glm::vec4& color)
	{
		color_picker_info.initial_color = color; 
		update();

		VecSlider<4>::set({ color.x * mult, color.y * mult, color.z * mult, color.w * mult });
	}

	void ColorPicker::update()
	{
		preview_panel->sprite_panel_info.color = color_picker_info.initial_color;
	}

}