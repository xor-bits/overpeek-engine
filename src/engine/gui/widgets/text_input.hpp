#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/text_edit.hpp"

#include <regex>



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	// text
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_any                            = R"regex(/^.*$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_multiline_any                  = R"regex(/^.*$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_letters_upper                  = R"regex(/^[A-Z]$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_letters_lower                  = R"regex(/^[a-z]$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_multiline_letters_upper        = R"regex(/^[A-Z]$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_multiline_letters_lower        = R"regex(/^[a-z]$/gm)regex";
	
	// numbers
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_integer_numbers                = R"regex(/^[-+]?\d*\.?\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_integer_numbers_empty          = R"regex(/^([-+]?\d*\.?\d+)?$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_float_numbers                  = R"regex(/^[-+]?\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_float_numbers_empty            = R"regex(/^([-+]?\d+)?$/gm)regex";
	
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_positive_integer_numbers       = R"regex(/^\+?\d*\.?\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_positive_integer_numbers_empty = R"regex(/^(\+?\d*\.?\d+)?$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_positive_float_numbers         = R"regex(/^\+?\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_positive_float_numbers_empty   = R"regex(/^(\+?\d+)?$/gm)regex";
	
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_negative_integer_numbers       = R"regex(/^-\d*\.?\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_negative_integer_numbers_empty = R"regex(/^(-\d*\.?\d+)?$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_negative_float_numbers         = R"regex(/^-\d+$/gm)regex";
	template<typename char_type> static constexpr std::basic_string_view<char_type> filter_negative_float_numbers_empty   = R"regex(/^(-\d+)?$/gm)regex";

	template<typename char_type>
	struct TextInputInfo
	{
		glm::vec2 align_text                           = oe::alignments::center_center;
		std::basic_string<char_type> initial_value     = {};
		std::basic_string<char_type> placeholder       = {};
		std::basic_string_view<char_type> regex_filter = filter_any<char_type>; // regex filter only works when the type of char_type is either char or wchar_t
		bool allow_newline                             = false;
		uint16_t font_size                             = 16;
		oe::utils::FontFile font_file                  = {}; // empty for gui default
		glm::vec4 color                                = oe::colors::dark_grey;
		glm::vec4 default_text_color                   = oe::colors::black;
		const oe::graphics::Sprite* sprite             = nullptr;
		
		WidgetInfo widget_info                         = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	template<typename char_type>
	struct TextInputHoverEvent
	{};

	template<typename char_type>
	struct TextInputUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
	};

	template<typename char_type>
	struct TextInputChangedEvent
	{
		char_type character;
		std::basic_string_view<char_type> value;
	};

	template<typename char_type, typename enable = void>
	class __TextInputBase;

	template<typename char_type>
	class __TextInputBase<char_type, std::enable_if_t<!std::is_same<char_type, char>::value && !std::is_same<char_type, wchar_t>::value>>
	{
	public:
		__TextInputBase(std::basic_string_view<char_type> str_view)
		{}
	};

	template<typename char_type>
	class __TextInputBase<char_type, std::enable_if_t<std::is_same<char_type, char>::value || std::is_same<char_type, wchar_t>::value>>
	{
	public:
		__TextInputBase(std::basic_string_view<char_type> str_view)
			: m_regex(str_view.data(), std::regex_constants::optimize)
		{}
	protected:
		std::basic_regex<char_type> m_regex;
	};

	template<typename char_type>
	class TextInput : public __TextInputBase<char_type>, public Widget
	{
	private:
		oe::graphics::BasicTextLabel<char_type>* m_label;
		std::unique_ptr<oe::graphics::Quad> m_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_bar_quad;

	public:
		TextInputInfo<char_type> m_text_input_info;
		std::basic_string<char_type>& m_value;
	
	private:
		oe::utils::stb_textedit<char_type> m_state;
		bool m_selected;

		std::chrono::high_resolution_clock::time_point m_timer_key_pressed;

	public:
		// window_handle is used for clipboard
		TextInput(Widget* parent, GUI& gui_manager, std::basic_string<char_type>& m_value_ref, const TextInputInfo<char_type>& text_input_info = {});
		TextInput(Widget* parent, GUI& gui_manager, const TextInputInfo<char_type>& text_input_info = {})
			: TextInput(parent, gui_manager, m_text_input_info.initial_value, text_input_info)
		{}

		virtual void virtual_toggle(bool enabled) override;
		void resetTimer() noexcept;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_button(const MouseButtonEvent& event);
		oe::utils::connect_guard m_cg_render;
		oe::utils::connect_guard m_cg_codepoint;
		oe::utils::connect_guard m_cg_key;
		oe::utils::connect_guard m_cg_button;
	};
}