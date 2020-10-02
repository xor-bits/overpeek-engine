#pragma once

#include "text_input.hpp"



namespace oe::gui
{

	struct NumberInputInfo
	{
		glm::vec2 align_text               = oe::alignments::center_center;
		float initial_value                = 0.0f;
		glm::vec2 value_bounds             = { -1.0f, 1.0f };
		uint16_t font_size                 = 16;
		oe::utils::FontFile font_file      = {}; // empty for gui default
		glm::vec4 color                    = oe::colors::dark_grey;
		glm::vec4 default_text_color       = oe::colors::black;
		const oe::graphics::Sprite* sprite = nullptr;

		interact_type_flags interact_flags = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;

		WidgetInfo widget_info             = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	class NumberInput : public TextInput
	{
	public:

	};
}