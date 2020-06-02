#pragma once

#include "widget.hpp"
#include "sprite_panel.hpp"



namespace oe::gui {

	typedef std::function<void(glm::vec4)> color_picker_callback;

	struct ColorPickerInfo {
		color_picker_callback callback             = nullptr;
		float min_value                            = -1.0f;
		float max_value                            = 1.0f;
		glm::vec4 initial_color                    = oe::colors::white;
		glm::vec4 background_color                 = oe::colors::dark_grey;
		glm::ivec2 size                            = { 200, 100 };
		const oe::graphics::Sprite* sprite         = nullptr; // must be set
		glm::vec2 offset_position                  = { 0, 0 };
		glm::vec2 align_parent                     = oe::alignments::center_center;
		glm::vec2 align_render                     = oe::alignments::center_center;
	};

	class Slider;
	class ColorPicker : public Widget {
	private:
		SpritePanel* preview_panel = nullptr;
	
	public:
		ColorPickerInfo color_picker_info;
		Slider* slider_r;
		Slider* slider_g;
		Slider* slider_b;
		Slider* slider_a;

	public:
		ColorPicker(GUI* gui_manager, const ColorPickerInfo& color_picker_info);

		const glm::vec4& get() const;
		void set(const glm::vec4& color);
		void update();
	};

}