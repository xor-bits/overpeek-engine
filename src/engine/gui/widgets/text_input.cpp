#include "text_input.h"

#include "engine/graphics/textLabel.h"
#include "engine/engine.h"
#include "engine/graphics/interface/window.h"

bool insertchars(std::string *obj, int i, char *chars, int n) {
	obj->insert(i, chars, n);
	return true;
}

#define STB_TEXTEDIT_STRING						std::string
#define STB_TEXTEDIT_CHARTYPE					char

#define STB_TEXTEDIT_STRINGLEN(obj)				obj->size()
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n)			NULL
#define STB_TEXTEDIT_GETWIDTH(obj,n,i)			NULL
#define STB_TEXTEDIT_GETCHAR(obj,i)				obj->at(i)
#define STB_TEXTEDIT_NEWLINE					'\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n)		obj->erase(i, n)
#define STB_TEXTEDIT_INSERTCHARS(obj,i,c,n)		insertchars(obj, i, c, n)
#define STB_TEXTEDIT_KEYTOTEXT(k)				k
#define STB_TEXTEDIT_K_UNDO						300
#define STB_TEXTEDIT_K_REDO						301
#define STB_TEXTEDIT_K_LEFT						302
#define STB_TEXTEDIT_K_RIGHT					303
#define STB_TEXTEDIT_K_DOWN						304
#define STB_TEXTEDIT_K_UP						305
#define STB_TEXTEDIT_K_DELETE					306
#define STB_TEXTEDIT_K_BACKSPACE				307
#define STB_TEXTEDIT_K_TEXTSTART				308	// ctrl home
#define STB_TEXTEDIT_K_TEXTEND					309 // ctrl end
#define STB_TEXTEDIT_K_LINESTART				310	// home
#define STB_TEXTEDIT_K_LINEEND					311 // end
#define STB_TEXTEDIT_K_WORDLEFT					312 // ctrl left
#define STB_TEXTEDIT_K_WORDRIGHT				313 // ctrl right
#define STB_TEXTEDIT_K_SHIFT					0x10000

#define STB_TEXTEDIT_IMPLEMENTATION
#include <stb_textedit.h>



namespace oe::gui {

	float timer_key_pressed = 0;
	void resetTimer() {
		timer_key_pressed = oe::utils::Clock::getSessionMillisecond() + 1000.0f;
	}
	
	TextInput::TextInput(oe::graphics::Window* window_handle, const glm::vec2& bounding_box_size) 
		: Widget(bounding_box_size)
		, m_string("")
		, m_selected(false)
		, m_color(0.7f, 0.7f, 0.7f, 1.0f)
		, m_sprite(nullptr)
		, m_font_size(0)
		, m_callback_changed(nullptr)
		, m_callback_newline(nullptr)
		, m_window_handle(window_handle)
	{
		m_state = new STB_TexteditState();
	}

	void TextInput::render(oe::graphics::Renderer& renderer) {
		// bounding box
		renderer.submit(m_render_position, m_size, m_sprite, m_color);

		// text
		oe::graphics::Text::submit(renderer, "<#000000>" + m_string, m_render_position, glm::vec2(m_font_size), oe::alignments::top_left);

		// vertical bar
		if (!m_selected) return;
		float input_x_bar = oe::graphics::Text::width(m_string.substr(0, reinterpret_cast<STB_TexteditState*>(m_state)->cursor), glm::vec2(m_font_size));
		if (timer_key_pressed > oe::utils::Clock::getSessionMillisecond()) {
			oe::graphics::Text::submit(renderer, "<#000000>|", m_render_position + glm::vec2(input_x_bar, 0.0f), glm::vec2(m_font_size), oe::alignments::top_left);
			return;
		}
		else {
			float time = oe::utils::Clock::getSessionMillisecond();
			if ((int)floor(time) % 2000 > 1000)
				oe::graphics::Text::submit(renderer, "<#000000>|", m_render_position + glm::vec2(input_x_bar, 0.0f), glm::vec2(m_font_size), oe::alignments::top_left);
			return;
		}
	}

	void TextInput::text(uint32_t codepoint, oe::modifiers mods) {
		if (!m_selected) return;

		stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), codepoint);
		resetTimer();

		if (m_callback_changed) m_callback_changed(m_string);
	}

	void TextInput::cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		if (button == oe::mouse_buttons::button_left && action == oe::actions::press) {
			if (cursor_window.x >= m_render_position.x &&
				cursor_window.x < m_render_position.x + m_size.x &&
				cursor_window.y >= m_render_position.y &&
				cursor_window.y < m_render_position.y + m_size.y
			/*if (*/ ) m_selected = true;
			else m_selected = false;
		}
	}

	void TextInput::key(oe::keys key, oe::actions action, oe::modifiers mods) {
		if (!m_selected) return;

		if (action != oe::actions::release) {
			if (key == oe::keys::key_backspace)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_BACKSPACE);
			else if (key == oe::keys::key_delete)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_DELETE);
			else if (key == oe::keys::key_left && mods == oe::modifiers::none)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_LEFT);
			else if (key == oe::keys::key_left && mods == oe::modifiers::control)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDLEFT);
			else if (key == oe::keys::key_right && mods == oe::modifiers::none)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_RIGHT);
			else if (key == oe::keys::key_right && mods == oe::modifiers::control)
				stb_textedit_key(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDRIGHT);
			else if (key == oe::keys::key_v && mods == oe::modifiers::control) { // ctrl + v
				std::string cb = m_window_handle->getClipboard();
				stb_textedit_paste(&m_string, reinterpret_cast<STB_TexteditState*>(m_state), cb.c_str(), cb.size());
			}
			else if (key == oe::keys::key_c && mods == oe::modifiers::control) { // ctrl + c
				std::string copied = m_string.substr(reinterpret_cast<STB_TexteditState*>(m_state)->select_start, reinterpret_cast<STB_TexteditState*>(m_state)->select_end - reinterpret_cast<STB_TexteditState*>(m_state)->select_start);
				spdlog::info(copied);
				m_window_handle->setClipboard(copied);
				stb_textedit_cut(&m_string, reinterpret_cast<STB_TexteditState*>(m_state));
			}
			else if (key == oe::keys::key_enter) {
				if (m_callback_newline) m_callback_newline(m_string);
			}

			resetTimer();
			if (m_callback_changed) m_callback_changed(m_string);
		}
	}

}