#pragma once

#include "widget.hpp"
#include "event.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/text_edit.hpp"

#include <regex>



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	// events
	template<typename char_type>
	struct BasicTextInputHoverEvent : BaseHoverEvent {};

	template<typename char_type>
	struct BasicTextInputUseEvent : BaseUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		std::basic_string_view<char_type> value;
	};

	template<typename char_type>
	struct BasicTextInputInputEvent
	{
		char32_t codepoint;
		std::basic_string_view<char_type> value;
	};



	template<typename char_type>
	class BasicTextInput : public Widget
	{
	public:
		// types
		using value_t = std::basic_string<char_type>;
		struct info_t
		{
			// characters
			size_t max_characters                                        = std::numeric_limits<size_t>::max();
			std::basic_string<char_type> initial_value                   = {};
			std::basic_string<char_type> placeholder                     = {};
			std::basic_string_view<char_type> whitelist                  = {};
			std::basic_string_view<char_type> blacklist                  = {};
			bool allow_newline                                           = false;
			// visuals
			glm::vec2 align_text                                         = oe::alignments::center_center;
			std::function<void(std::basic_string<char_type>&)> formatter = &default_formatter;
			uint16_t font_size                                           = 16;
			oe::utils::FontFile font_file                                = {}; // empty for gui default
			glm::vec4 color                                              = oe::colors::dark_grey;
			glm::vec4 selection_color                                    = { 0.5f, 0.5f, 1.0f, 0.4f };
			glm::vec4 default_text_color                                 = oe::colors::white;
			const oe::graphics::Sprite* sprite                           = nullptr;
			// base
			Widget::info_t widget_info                                   = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
		


			//
			static inline void default_formatter(std::basic_string<char_type>& /* val */) {}
		};
		
	private:
		oe::graphics::BasicTextLabel<char_type>* m_label;
		std::unique_ptr<oe::graphics::Quad> m_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_quad;
		std::unique_ptr<oe::graphics::Quad> m_text_bar_quad;

		std::array<std::unique_ptr<oe::graphics::Quad>, 3> m_text_selection_quads;
		
		glm::ivec2 m_text_label_pos;

	public:
		info_t m_text_input_info;
		value_t& m_value;
		bool m_selected = false;
	
	private:
		oe::utils::stb_textedit<char_type> m_state;
		bool m_dragging = false;
		std::chrono::milliseconds m_double_click_delay = std::chrono::milliseconds(500);

		std::chrono::high_resolution_clock::time_point m_timer_key_pressed;

	public:
		BasicTextInput(Widget* parent, GUI& gui_manager, const info_t& text_input_info, value_t& m_value_ref);
		BasicTextInput(Widget* parent, GUI& gui_manager, const info_t& text_input_info)
			: BasicTextInput(parent, gui_manager, text_input_info, m_text_input_info.initial_value)
		{}

		[[nodiscard]] int& cursor() const noexcept;
		[[nodiscard]] const std::tuple<int&, int&>& selection() const noexcept;

		void virtual_toggle(bool enabled) override;
		void resetTimer() noexcept;
		void reformat() noexcept;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		oe::utils::connect_guard m_cg_render;
		oe::utils::connect_guard m_cg_codepoint;
		oe::utils::connect_guard m_cg_key;
		oe::utils::connect_guard m_cg_cursor;
		oe::utils::connect_guard m_cg_button;
	};

	using TextInput = BasicTextInput<char>;
	using wTextInput = BasicTextInput<wchar_t>;
	using u16TextInput = BasicTextInput<char16_t>;
	using u32TextInput = BasicTextInput<char32_t>;
}