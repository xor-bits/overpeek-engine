#pragma once

#include <string>
#include <tuple>
#include <glm/glm.hpp>
#include "engine/graphics/font.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::utils
{
	enum text_flags
	{
		allow_newline = 1>>0
	};

	[[nodiscard]] constexpr inline text_flags operator|(text_flags a, text_flags b)
	{
		return static_cast<text_flags>(static_cast<int>(a) | static_cast<int>(b));
	}
	
	template<typename char_type>
	struct stb_textedit
	{
		void* m_state;
		int& m_cursor;
		std::tuple<int&, int&> m_selection;

		std::function<void(std::string&)> m_paste_from_clipboard;
		std::function<void(const std::string&)> m_copy_to_clipboard;

		stb_textedit(text_flags f = static_cast<text_flags>(0));
		~stb_textedit();
		void key(std::basic_string<char_type>& string, oe::graphics::Font& font, uint32_t key, oe::modifiers mods = oe::modifiers::none);
		void key(std::basic_string<char_type>& string, oe::graphics::Font& font, oe::keys key, oe::modifiers mods = oe::modifiers::none);
		void flush(); // flush redo que without key input
		void clamp(std::basic_string<char_type>& string); // fix cursor and selection positions
		void click(std::basic_string<char_type>& string, oe::graphics::Font& font, const glm::ivec2& cursor);
		void drag(std::basic_string<char_type>& string, oe::graphics::Font& font, const glm::ivec2& cursor);
		[[nodiscard]] int& cursor() const noexcept;
		[[nodiscard]] const std::tuple<int&, int&>& selection() const noexcept;
	};
}