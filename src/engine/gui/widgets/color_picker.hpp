#pragma once

#include "widget.hpp"
#include "vec_slider.hpp"



namespace oe::gui { class Slider; class SpritePanel; }
namespace oe::graphics { struct Sprite; }

namespace oe::gui
{
	struct ColorPickerInfo
	{
		uint8_t draw_value                 = 2; // (false/0) = no draw, (true/1) = draw 0.0-1.0, 2 = draw 0-256
		glm::vec4 initial_color            = oe::colors::white;
		glm::vec4 background_color         = oe::colors::dark_grey;
		const oe::graphics::Sprite* sprite = nullptr; // must be set

		WidgetInfo widget_info = { { 200, 100 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	struct ColorPickerHoverEvent
	{};

	struct ColorPickerUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		glm::vec4 value;
	};

	class ColorPicker : public VecSlider<4> 
	{
	public:
		ColorPickerInfo color_picker_info;
		ColorPickerHoverEvent event_hover_latest;
		ColorPickerUseEvent event_use_latest;

	private:
		SpritePanel* preview_panel = nullptr;
		const float mult;

	public:
		ColorPicker(const ColorPickerInfo& color_picker_info);
		~ColorPicker();

		const glm::vec4& get() const;
		void set(const glm::vec4& color);
		void update();

	private:
		void on_vec_slider_hover(const VecSliderHoverEvent<4>& e);
		void on_vec_slider_use(const VecSliderUseEvent<4>& e);
	};

}