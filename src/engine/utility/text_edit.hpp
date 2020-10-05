#pragma once

#include <string>



namespace oe::utils
{	
	enum text_keys
	{
		undo      = -6000000,
		redo      = -5000000,
		left      = -4000000,
		right     = -3000000,
		c_left    = -2000000,
		c_right   = -1000000,
		down      = -900000,
		up        = -800000,
		del       = -700000,
		backspace = -600000,
		c_home    = -500000,
		c_end     = -400000,
		home      = -300000,
		end       = -200000,
		shift     = -100000,
	};

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

		void* state;

		stb_textedit(text_flags f = static_cast<text_flags>(0));
		~stb_textedit();
		void key(std::basic_string<char_type>& string, int key);
		void cut(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard);
		void copy(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard);
		void paste(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard);
		void flush();
		[[nodiscard]] int& cursor() const noexcept;
	};
}