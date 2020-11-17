#ifdef STB_TEXTEDIT_KEYTYPE
#undef STB_TEXTEDIT_KEYTYPE
#endif
#define STB_TEXTEDIT_KEYTYPE                    uint32_t
#define STB_TEXTEDIT_CHARTYPE					char_type
#define STB_TEXTEDIT_STRING						oe::utils::stb_textedit<char_type>::text_type
#include <stb_textedit.h>
#include <cstdint>
#include "engine/utility/extra.hpp"

static size_t* last_max_characters = nullptr;



static bool insertchars(STB_TEXTEDIT_STRING *obj, uint32_t i, char_type *chars, uint32_t n)
{
	if(last_max_characters && obj->m_string.size() + n > *last_max_characters)
		return false;
	obj->m_string.insert(i, chars, n);
	return true;
}

static float getwidth(STB_TEXTEDIT_STRING *obj, uint32_t /* n */, uint32_t i)
{
	float advance = 0.0f;
	if(obj->m_cache.datapoints.size() != 0)
		advance = obj->m_cache.datapoints.at(std::min<size_t>(obj->m_cache.datapoints.size(), i + 1)).offset.x - obj->m_cache.datapoints.at(std::min<size_t>(obj->m_cache.datapoints.size(), i)).offset.x;
	return advance;
}

static void layoutrow(StbTexteditRow* r, STB_TEXTEDIT_STRING *obj, uint32_t n)
{
	const size_t find_nl = [](size_t pos, size_t last){
		if(pos == std::basic_string<char_type>::npos)
			return last;
		return pos;
	}(obj->m_string.find(static_cast<char_type>('\n'), n), obj->m_string.size() - 1);
	const size_t count = find_nl - n + 1;
	const float width = oe::graphics::BasicText<char_type>::offset_to_char(obj->m_cache, find_nl == std::basic_string<char_type>::npos ? obj->m_cache.datapoints.size() - 1 : find_nl).x;
	const float height = obj->m_cache.scaling.y;

	r->ymax = height;
	r->ymin = 0.0f;
	r->baseline_y_delta = height;
	r->num_chars = static_cast<int>(std::min(count, static_cast<size_t>(std::numeric_limits<int>::max())));
	r->x0 = 0.0f;
	r->x1 = width;
}

#define KEYDOWN_BIT                             0x8000000

#define STB_TEXTEDIT_STRINGLEN(obj)				static_cast<int>(std::min(obj->m_string.size(), static_cast<size_t>(std::numeric_limits<int>::max())))
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n)			layoutrow(r, obj, n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i)			getwidth(obj, n, i)
#define STB_TEXTEDIT_GETCHAR(obj,i)				obj->m_string.at(i)
#define STB_TEXTEDIT_NEWLINE					'\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n)		obj->m_string.erase(i, n)
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

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define STB_TEXTEDIT_IMPLEMENTATION
#include <stb_textedit.h>

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif

namespace oe::utils
{
	template<>
	stb_textedit<char_type>::stb_textedit(std::basic_string<char_type>& string, const oe::graphics::text_render_cache& cache, size_t& max_chars, text_flags f)
		: m_state(new STB_TexteditState())
		, m_cursor(reinterpret_cast<STB_TexteditState*>(m_state)->cursor)
		, m_selection({ reinterpret_cast<STB_TexteditState*>(m_state)->select_end, reinterpret_cast<STB_TexteditState*>(m_state)->select_start })
		, m_max_characters(max_chars)
		, m_string(string, cache)
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
	void stb_textedit<char_type>::key(uint32_t key, oe::modifiers mods)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &m_max_characters;

		if(!key)
			return;
		
		if(static_cast<bool>(mods & oe::modifiers::control))
			key |= STB_TEXTEDIT_K_CONTROL;
		if(static_cast<bool>(mods & oe::modifiers::shift))
			key |= STB_TEXTEDIT_K_SHIFT;

		if (key == K_COPY)
		{
			auto clipboard = m_string.m_string.substr(std::get<0>(m_selection), std::get<1>(m_selection) - std::get<0>(m_selection));
			m_copy_to_clipboard(oe::utils::convertUTF<std::string>(clipboard));
		}
		else if (key == K_CUT)
		{
			auto clipboard = m_string.m_string.substr(std::get<0>(m_selection), std::get<1>(m_selection) - std::get<0>(m_selection));
			m_copy_to_clipboard(oe::utils::convertUTF<std::string>(clipboard));
			stb_textedit_cut(&m_string, state);
		}
		else if (key == K_PASTE)
		{
			std::string clipboard;
			m_paste_from_clipboard(clipboard);
			auto clipboard_chartype = oe::utils::convertUTF<std::basic_string<char_type>>(clipboard);
			stb_textedit_paste(&m_string, state, clipboard_chartype.data(), static_cast<int>(std::min(clipboard_chartype.size(), static_cast<size_t>(std::numeric_limits<int>::max()))));
		}
		
		stb_textedit_key(&m_string, state, key);
	}

	constexpr uint32_t key_enum_to_code(oe::keys key_enum, oe::modifiers mods)
	{
		switch (key_enum)
		{
		default:
			{
				if(key_enum == oe::keys::key_x && mods == oe::modifiers::control)
					return K_CUT;
				else if(key_enum == oe::keys::key_c && mods == oe::modifiers::control)
					return K_COPY;
				else if(key_enum == oe::keys::key_v && mods == oe::modifiers::control)
					return K_PASTE;
			}
			return 0;
		case oe::keys::key_left:
			return STB_TEXTEDIT_K_LEFT;
		case oe::keys::key_right:
			return STB_TEXTEDIT_K_RIGHT;
		case oe::keys::key_up:
			return STB_TEXTEDIT_K_UP;
		case oe::keys::key_down:
			return STB_TEXTEDIT_K_DOWN;
		case oe::keys::key_home:
			return STB_TEXTEDIT_K_LINESTART;
		case oe::keys::key_end:
			return STB_TEXTEDIT_K_LINEEND;
		case oe::keys::key_delete:
			return STB_TEXTEDIT_K_DELETE;
		case oe::keys::key_backspace:
			return STB_TEXTEDIT_K_BACKSPACE;
		case oe::keys::key_insert:
			return STB_TEXTEDIT_K_INSERT;
		case oe::keys::key_page_up:
			return STB_TEXTEDIT_K_PGUP;
		case oe::keys::key_page_down:
			return STB_TEXTEDIT_K_PGDOWN;
		case oe::keys::key_enter:
			return static_cast<uint32_t>('\n');
		}
	}
	
	template<>
	void stb_textedit<char_type>::key(oe::keys key_enum, oe::modifiers mods)
	{
		last_max_characters = &m_max_characters;

		uint32_t code = key_enum_to_code(key_enum, mods);
		key(code, mods);
	}

	template<>
	void stb_textedit<char_type>::flush()
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &m_max_characters;
		stb_textedit_flush_redo(&state->undostate);
	}
	
	template<>
	void stb_textedit<char_type>::clamp()
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &m_max_characters;
		stb_textedit_clamp(&m_string, state);
	}
	
	template<>
	void stb_textedit<char_type>::click(const glm::vec2& cursor)
	{
		/* spdlog::debug("c:{}", cursor); */
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &m_max_characters;
		stb_textedit_click(&m_string, state, cursor.x, cursor.y);
	}

	template<>
	void stb_textedit<char_type>::drag(const glm::vec2& cursor)
	{
		auto state = reinterpret_cast<STB_TexteditState*>(m_state);
		last_max_characters = &m_max_characters;
		stb_textedit_drag(&m_string, state, cursor.x, cursor.y);
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