bool insertchars(std::basic_string<char_type> *obj, int i, char_type *chars, int n)
{
	obj->insert(i, chars, n);
	return true;
}

#define STB_TEXTEDIT_STRINGLEN(obj)				obj->size()
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n)			NULL
#define STB_TEXTEDIT_GETWIDTH(obj,n,i)			NULL
#define STB_TEXTEDIT_GETCHAR(obj,i)				obj->at(i)
#define STB_TEXTEDIT_NEWLINE					'\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n)		obj->erase(i, n)
#define STB_TEXTEDIT_INSERTCHARS(obj,i,c,n)		insertchars(obj, i, c, n)
#define STB_TEXTEDIT_KEYTOTEXT(k)				k
#define STB_TEXTEDIT_K_UNDO						oe::utils::text_keys::undo
#define STB_TEXTEDIT_K_REDO						oe::utils::text_keys::redo
#define STB_TEXTEDIT_K_WORDLEFT					oe::utils::text_keys::c_left
#define STB_TEXTEDIT_K_WORDRIGHT				oe::utils::text_keys::c_right
#define STB_TEXTEDIT_K_LEFT						oe::utils::text_keys::left
#define STB_TEXTEDIT_K_RIGHT					oe::utils::text_keys::right
#define STB_TEXTEDIT_K_DOWN						oe::utils::text_keys::down
#define STB_TEXTEDIT_K_UP						oe::utils::text_keys::up
#define STB_TEXTEDIT_K_DELETE					oe::utils::text_keys::del
#define STB_TEXTEDIT_K_BACKSPACE				oe::utils::text_keys::backspace
#define STB_TEXTEDIT_K_TEXTSTART				oe::utils::text_keys::c_home
#define STB_TEXTEDIT_K_TEXTEND					oe::utils::text_keys::c_end
#define STB_TEXTEDIT_K_LINESTART				oe::utils::text_keys::home
#define STB_TEXTEDIT_K_LINEEND					oe::utils::text_keys::end
#define STB_TEXTEDIT_K_SHIFT					oe::utils::text_keys::shift

#define STB_TEXTEDIT_CHARTYPE					char_type
#define STB_TEXTEDIT_STRING						std::basic_string<char_type>
#define STB_TEXTEDIT_IMPLEMENTATION
#include <stb_textedit.h>

namespace oe::utils
{
	template<>
	stb_textedit<char_type>::stb_textedit(text_flags f)
	{
		auto _state = new STB_TexteditState();;
		state = _state;

		_state->cursor = 0;
		_state->single_line = !(f | text_flags::allow_newline);
	}

	template<>
	stb_textedit<char_type>::~stb_textedit()
	{
		delete reinterpret_cast<STB_TexteditState*>(state);
	}
	
	template<>
	void stb_textedit<char_type>::key(std::basic_string<char_type>& string, int key)
	{
		stb_textedit_key(&string, reinterpret_cast<STB_TexteditState*>(state), key);
	}
	
	template<>
	void stb_textedit<char_type>::cut(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard)
	{
		copy(string, clipboard);
		stb_textedit_cut(&string, reinterpret_cast<STB_TexteditState*>(state));
	}
	
	template<>
	void stb_textedit<char_type>::copy(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard)
	{
		const auto& start = reinterpret_cast<STB_TexteditState*>(state)->select_start;
		const auto& end = reinterpret_cast<STB_TexteditState*>(state)->select_end;
		clipboard = string.substr(start, end - start);
	}
	
	template<>
	void stb_textedit<char_type>::paste(std::basic_string<char_type>& string, std::basic_string<char_type>& clipboard)
	{
		stb_textedit_paste(&string, reinterpret_cast<STB_TexteditState*>(state), clipboard.data(), clipboard.size());
	}

	template<>
	void stb_textedit<char_type>::flush()
	{
		stb_textedit_flush_redo(&reinterpret_cast<STB_TexteditState*>(state)->undostate);
	}
	
	template<>
	[[nodiscard]] int& stb_textedit<char_type>::cursor() const noexcept
	{
		return reinterpret_cast<STB_TexteditState*>(state)->cursor;
	}
}