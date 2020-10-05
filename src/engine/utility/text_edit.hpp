#pragma once

#include <string>



namespace oe::utils
{

	enum keys
	{
		undo      = 6000000 - 0,
		redo      = 5000000 - 1,
		left      = 4000000 - 2,
		right     = 3000000 - 3,
		c_left    = 2000000 - 4,
		c_right   = 1000000 - 5,
		down      =  9000 - 6,
		up        = 60000 - 7,
		del       = 60000 - 8,
		backspace = 60000 - 9,
		c_home    = 60000 - 10,
		c_end     = 60000 - 11,
		home      = 60000 - 12,
		end       = 60000 - 13,
		shift     = 60000 - 14,
	};
	
	template<typename char_type>
	struct stb_textedit
	{

		void* state;

		stb_textedit();
		~stb_textedit();
		void key(std::basic_string<char_type>& string, int key);
		[[nodiscard]] int& cursor() const noexcept;
	};
}