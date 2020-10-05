#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/text_edit.hpp"

#include <regex>



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	template<typename char_type>
	struct BasicTextInputInfo
	{
		glm::vec2 align_text                                         = oe::alignments::center_center;
		std::basic_string<char_type> initial_value                   = {};
		std::basic_string<char_type> placeholder                     = {};
		std::basic_string_view<char_type> whitelist                  = {};
		std::basic_string_view<char_type> blacklist                  = {};
		std::function<void(std::basic_string<char_type>&)> formatter = &default_formatter;
		bool allow_newline                                           = false;
		uint16_t font_size                                           = 16;
		oe::utils::FontFile font_file                                = {}; // empty for gui default
		glm::vec4 color                                              = oe::colors::dark_grey;
		glm::vec4 default_text_color                                 = oe::colors::black;
		const oe::graphics::Sprite* sprite                           = nullptr;
		
		WidgetInfo widget_info                                       = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	
		//
		static void default_formatter(std::basic_string<char_type>& val) {}
	};

	template<typename char_type>
	struct BasicTextInputHoverEvent
	{};

	template<typename char_type>
	struct BasicTextInputUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
	};

	template<typename char_type>
	struct BasicTextInputChangedEvent
	{
		char32_t codepoint;
		std::basic_string_view<char_type> value;
	};

	template<typename char_type>
	class BasicTextInput : public Widget
	{
	private:
		oe::graphics::BasicTextLabel<char_type>* m_label;
		std::unique_ptr<oe::graphics::Quad> m_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_bar_quad;

	public:
		BasicTextInputInfo<char_type> m_text_input_info;
		std::basic_string<char_type>& m_value;
	
	private:
		oe::utils::stb_textedit<char_type> m_state;
		bool m_selected;

		std::chrono::high_resolution_clock::time_point m_timer_key_pressed;

	public:
		// window_handle is used for clipboard
		BasicTextInput(Widget* parent, GUI& gui_manager, std::basic_string<char_type>& m_value_ref, const BasicTextInputInfo<char_type>& text_input_info = {});
		BasicTextInput(Widget* parent, GUI& gui_manager, const BasicTextInputInfo<char_type>& text_input_info = {})
			: BasicTextInput(parent, gui_manager, m_text_input_info.initial_value, text_input_info)
		{}

		virtual void virtual_toggle(bool enabled) override;
		void resetTimer() noexcept;
		void reformat() noexcept;
	
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

	using TextInput = BasicTextInput<char>;
	using wTextInput = BasicTextInput<wchar_t>;
	using u16TextInput = BasicTextInput<char16_t>;
	using u32TextInput = BasicTextInput<char32_t>;
	
	using TextInputInfo = BasicTextInputInfo<char>;
	using wTextInputInfo = BasicTextInputInfo<wchar_t>;
	using u16TextInputInfo = BasicTextInputInfo<char16_t>;
	using u32TextInputInfo = BasicTextInputInfo<char32_t>;

	using TextInputHoverEvent = BasicTextInputHoverEvent<char>;
	using wTextInputHoverEvent = BasicTextInputHoverEvent<wchar_t>;
	using u16TextInputHoverEvent = BasicTextInputHoverEvent<char16_t>;
	using u32TextInputHoverEvent = BasicTextInputHoverEvent<char32_t>;

	using TextInputUseEvent = BasicTextInputUseEvent<char>;
	using wTextInputUseEvent = BasicTextInputUseEvent<wchar_t>;
	using u16TextInputUseEvent = BasicTextInputUseEvent<char16_t>;
	using u32TextInputUseEvent = BasicTextInputUseEvent<char32_t>;

	using TextInputChangedEvent = BasicTextInputChangedEvent<char>;
	using wTextInputChangedEvent = BasicTextInputChangedEvent<wchar_t>;
	using u16TextInputChangedEvent = BasicTextInputChangedEvent<char16_t>;
	using u32TextInputChangedEvent = BasicTextInputChangedEvent<char32_t>;
}