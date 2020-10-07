#define STB_TEXTEDIT_CHARTYPE					char_type
#define STB_TEXTEDIT_KEYTYPE                    uint32_t
#define STB_TEXTEDIT_STRING						std::basic_string<char_type>
#include <stb_textedit.h>
#include "engine/utility/extra.hpp"

static oe::graphics::Font* last_font = nullptr;
static size_t* last_max_characters = nullptr;
static bool insertchars(std::basic_string<char_type> *obj, uint32_t i, char_type *chars, uint32_t n)
{
	if(last_max_characters && obj->size() + n > *last_max_characters)
		return false;
	obj->insert(i, chars, n);
	return true;
}

static float getwidth(std::basic_string<char_type> *obj, uint32_t n, uint32_t i)
{
	float advance = 0.0f;
	if(!last_font)
		return advance;
	
	advance = last_font->getGlyph(obj->at(i))->advance.x * last_font->getResolution();
	// spdlog::debug("i:{}, c:{}, advance:{}, font:{:X}",n,(char)obj->at(i),advance,(size_t)last_font);
	return advance;
}

static void layoutrow(StbTexteditRow* r, std::basic_string<char_type> *obj, uint32_t n)
{
	if(!last_font)
		return;

	const size_t find_nl = obj->find(static_cast<char_type>('\n'), n);
	const size_t first = n;
	const size_t count = find_nl == std::basic_string<char_type>::npos ? obj->size() - n : find_nl - n + 1;
	std::basic_string_view<char_type> line = { obj->data() + first, count };

	const auto text_size = oe::graphics::BasicText<char_type>::size(*last_font, oe::graphics::text_render_input<char_type>{ line }, glm::vec2{ static_cast<float>(last_font->getResolution()) });

	r->ymax = static_cast<float>(last_font->getResolution());
	r->ymin = 0.0f;
	r->baseline_y_delta = static_cast<float>(last_font->getResolution());
	r->num_chars = count;
	r->x0 = 0.0f;
	r->x1 = text_size.x;
	// spdlog::debug("n:{}, font:{:X}, ymax:{}, ymin:{}, baseline_y_delta:{}, num_chars:{}, x0:{}, x1:{}",n,(size_t)last_font,r->ymax,r->ymin,r->baseline_y_delta,r->num_chars,r->x0,r->x1);
}

#define KEYDOWN_BIT                             0x8000000

#define STB_TEXTEDIT_STRINGLEN(obj)				obj->size()
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n)			layoutrow(r, obj, n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i)			getwidth(obj, n, i)
#define STB_TEXTEDIT_GETCHAR(obj,i)				obj->at(i)
#define STB_TEXTEDIT_NEWLINE					'\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n)		obj->erase(i, n)
#define STB_TEXTEDIT_INSERTCHARS(obj,i,c,n)		insertchars(obj, i, c, n)
#define STB_TEXTEDIT_KEYTOTEXT(k)				(((key) & KEYDOWN_BIT) ? 0 : (key))

#define STB_TEXTEDIT_K_SHIFT                    0x40000000
#define STB_TEXTEDIT_K_CONTROL                  0x20000000
#define STB_TEXTEDIT_K_LEFT                     (KEYDOWN_BIT | 1) // actually use VK_LEFT, SDLK_LEFT, etc
#define STB_TEXTEDIT_K_RIGHT                    (KEYDOWN_BIT | 2) // VK_RIGHT
#define STB_TEXTEDIT_K_UP                       (KEYDOWN_BIT | 3) // VK_UP
#define STB_TEXTEDIT_K_DOWN                     (KEYDOWN_BIT | 4) // VK_DOWN
#define STB_TEXTEDIT_K_LINESTART                (KEYDOWN_BIT | 5) // VK_HOME
#define STB_TEXTEDIT_K_LINEEND                  (KEYDOWN_BIT | 6) // VK_END
#define STB_TEXTEDIT_K_TEXTSTART                (STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_TEXTEND                  (STB_TEXTEDIT_K_LINEEND   | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_DELETE                   (KEYDOWN_BIT | 7) // VK_DELETE
#define STB_TEXTEDIT_K_BACKSPACE                (KEYDOWN_BIT | 8) // VK_BACKSPACE
#define STB_TEXTEDIT_K_UNDO                     (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'z')
#define STB_TEXTEDIT_K_REDO                     (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'y')
#define STB_TEXTEDIT_K_INSERT                   (KEYDOWN_BIT | 9) // VK_INSERT
#define STB_TEXTEDIT_K_WORDLEFT                 (STB_TEXTEDIT_K_LEFT  | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_WORDRIGHT                (STB_TEXTEDIT_K_RIGHT | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_PGUP                     (KEYDOWN_BIT | 10) // VK_PGUP -- not implemented
#define STB_TEXTEDIT_K_PGDOWN                   (KEYDOWN_BIT | 11) // VK_PGDOWN -- not implemented
#define K_COPY                                  (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'c')
#define K_CUT                                   (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'x')
#define K_PASTE                                 (KEYDOWN_BIT | STB_TEXTEDIT_K_CONTROL | 'v')

#define STB_TEXTEDIT_IMPLEMENTATION
#include <stb_textedit.h>

namespace oe::utils
{
	template<>
	stb_textedit<char_type>::stb_textedit(size_t& max_chars, text_flags f)
		: m_state(new STB_TexteditState())
		, m_cursor(reinterpret_cast<STB_TexteditState*>(m_state)->cursor)
		, m_selection({ reinterpret_cast<STB_TexteditState*>(m_state)->select_end, reinterpret_cast<STB_TexteditState*>(m_state)->select_start })
		, max_characters(max_chars)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		state->single_line = !(f | text_flags::allow_newline);
	}

	template<>
	stb_textedit<char_type>::~stb_textedit()
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		delete state;
	}
	
	template<>
	void stb_textedit<char_type>::key(std::basic_string<char_type>& string, oe::graphics::Font& font, uint32_t key, oe::modifiers mods)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_font = &font;
		last_max_characters = &max_characters;

		if(!key)
			return;
		
		if(mods & oe::modifiers::control)
			key |= STB_TEXTEDIT_K_CONTROL;
		if(mods & oe::modifiers::shift)
			key |= STB_TEXTEDIT_K_SHIFT;

		if (key == K_COPY)
		{
			auto clipboard = string.substr(std::get<0>(m_selection), std::get<1>(m_selection) - std::get<0>(m_selection));
			m_copy_to_clipboard(oe::utils::convertUTF<std::string>(clipboard));
		}
		else if (key == K_CUT)
		{
			auto clipboard = string.substr(std::get<0>(m_selection), std::get<1>(m_selection) - std::get<0>(m_selection));
			m_copy_to_clipboard(oe::utils::convertUTF<std::string>(clipboard));
			stb_textedit_cut(&string, state);
		}
		else if (key == K_PASTE)
		{
			std::string clipboard;
			m_paste_from_clipboard(clipboard);
			auto clipboard_chartype = oe::utils::convertUTF<std::basic_string<char_type>>(clipboard);
			stb_textedit_paste(&string, state, clipboard_chartype.data(), clipboard_chartype.size());
		}
		
		stb_textedit_key(&string, state, key);
	}
	
	template<>
	void stb_textedit<char_type>::key(std::basic_string<char_type>& string, oe::graphics::Font& font, oe::keys key_enum, oe::modifiers mods)
	{
		last_font = &font;
		last_max_characters = &max_characters;

		uint32_t code = 0;
		switch (key_enum)
		{
		default:
			break;

		case oe::keys::key_left:
			code = STB_TEXTEDIT_K_LEFT;
			break;
		case oe::keys::key_right:
			code = STB_TEXTEDIT_K_RIGHT;
			break;
		case oe::keys::key_up:
			code = STB_TEXTEDIT_K_UP;
			break;
		case oe::keys::key_down:
			code = STB_TEXTEDIT_K_DOWN;
			break;
		case oe::keys::key_home:
			code = STB_TEXTEDIT_K_LINESTART;
			break;
		case oe::keys::key_end:
			code = STB_TEXTEDIT_K_LINEEND;
			break;
		case oe::keys::key_delete:
			code = STB_TEXTEDIT_K_DELETE;
			break;
		case oe::keys::key_backspace:
			code = STB_TEXTEDIT_K_BACKSPACE;
			break;
		case oe::keys::key_insert:
			code = STB_TEXTEDIT_K_INSERT;
			break;
		case oe::keys::key_page_up:
			code = STB_TEXTEDIT_K_PGUP;
			break;
		case oe::keys::key_page_down:
			code = STB_TEXTEDIT_K_PGDOWN;
			break;
		case oe::keys::key_c:
			code = K_COPY;
			break;
		case oe::keys::key_x:
			code = K_CUT;
			break;
		case oe::keys::key_v:
			code = K_PASTE;
			break;
		case oe::keys::key_enter:
			code = '\n';
			break;
		}
		
		key(string, font, code, mods);
	}

	template<>
	void stb_textedit<char_type>::flush()
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &max_characters;
		stb_textedit_flush_redo(&state->undostate);
	}
	
	template<>
	void stb_textedit<char_type>::clamp(std::basic_string<char_type>& string)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &max_characters;
		stb_textedit_clamp(&string, state);
	}
	
	template<>
	void stb_textedit<char_type>::click(std::basic_string<char_type>& string, oe::graphics::Font& font, const glm::ivec2& cursor)
	{
		// spdlog::debug("c:{}", cursor);
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_font = &font;
		last_max_characters = &max_characters;
		stb_textedit_click(&string, state, cursor.x, cursor.y);
	}

	template<>
	void stb_textedit<char_type>::drag(std::basic_string<char_type>& string, oe::graphics::Font& font, const glm::ivec2& cursor)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_font = &font;
		last_max_characters = &max_characters;
		stb_textedit_drag(&string, state, cursor.x, cursor.y);
	}
	
	template<>
	[[nodiscard]] int& stb_textedit<char_type>::cursor() const noexcept
	{
		return m_cursor;
	}
	
	template<>
	[[nodiscard]] const std::tuple<int&, int&>& stb_textedit<char_type>::selection() const noexcept
	{
		return m_selection;
	}
}