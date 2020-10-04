#pragma once

#include "text_input.hpp"



namespace oe::gui
{

	struct NumberInputInfo
	{
		glm::vec2 align_text               = oe::alignments::center_center;
		float initial_value                = 0.0f;
		glm::vec2 value_bounds             = { std::numeric_limits<float>::min(), std::numeric_limits<float>::max() };
		uint16_t font_size                 = 16;
		oe::utils::FontFile font_file      = {}; // empty for gui default
		glm::vec4 color                    = oe::colors::dark_grey;
		glm::vec4 default_text_color       = oe::colors::black;
		const oe::graphics::Sprite* sprite = nullptr;

		interact_type_flags interact_flags = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;

		WidgetInfo widget_info             = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	class NumberInput : public TextInput<char>
	{
	public:
		NumberInputInfo m_number_input_info;
	
	private:
		float& m_value;
		
		static TextInputInfo<char> getTextInputInfo(const NumberInputInfo& number_input_info) noexcept
		{
			TextInputInfo<char> info;
			info.align_text = number_input_info.align_text;
			info.font_size = number_input_info.font_size;
			info.font_file = number_input_info.font_file;
			info.color = number_input_info.color;
			info.default_text_color = number_input_info.default_text_color;
			info.sprite = number_input_info.sprite;
			info.widget_info = number_input_info.widget_info;
			return info;
		}

	public:
		NumberInput(Widget* parent, GUI& gui_manager, float& value_ref, const NumberInputInfo& number_input_info = {})
			: TextInput<char>(parent, gui_manager, getTextInputInfo(number_input_info))
			, m_number_input_info(number_input_info)
			, m_value(value_ref)
		{

		}
		NumberInput(Widget* parent, GUI& gui_manager, const NumberInputInfo& number_input_info = {}) : NumberInput(parent, gui_manager, m_number_input_info.initial_value, number_input_info) {}
	};
}