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
#define STB_TEXTEDIT_K_UNDO						oe::utils::keys::undo
#define STB_TEXTEDIT_K_REDO						oe::utils::keys::redo
#define STB_TEXTEDIT_K_WORDLEFT					oe::utils::keys::c_left
#define STB_TEXTEDIT_K_WORDRIGHT				oe::utils::keys::c_right
#define STB_TEXTEDIT_K_LEFT						oe::utils::keys::left
#define STB_TEXTEDIT_K_RIGHT					oe::utils::keys::right
#define STB_TEXTEDIT_K_DOWN						oe::utils::keys::down
#define STB_TEXTEDIT_K_UP						oe::utils::keys::up
#define STB_TEXTEDIT_K_DELETE					oe::utils::keys::del
#define STB_TEXTEDIT_K_BACKSPACE				oe::utils::keys::backspace
#define STB_TEXTEDIT_K_TEXTSTART				oe::utils::keys::c_home
#define STB_TEXTEDIT_K_TEXTEND					oe::utils::keys::c_end
#define STB_TEXTEDIT_K_LINESTART				oe::utils::keys::home
#define STB_TEXTEDIT_K_LINEEND					oe::utils::keys::end
#define STB_TEXTEDIT_K_SHIFT					oe::utils::keys::shift

#define STB_TEXTEDIT_CHARTYPE					char_type
#define STB_TEXTEDIT_STRING						std::basic_string<char_type>
#define STB_TEXTEDIT_IMPLEMENTATION
#include <stb_textedit.h>

namespace oe::utils
{
	template<>
	stb_textedit<char_type>::stb_textedit()
	{
		state = new STB_TexteditState();
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
	[[nodiscard]] int& stb_textedit<char_type>::cursor() const noexcept
	{
		return reinterpret_cast<STB_TexteditState*>(state)->cursor;
	}
}