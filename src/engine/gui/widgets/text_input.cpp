#include "text_input.hpp"

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/utility/clock.hpp"

bool insertchars(std::u32string *obj, int i, char32_t *chars, int n) {
	obj->insert(i, chars, n);
	return true;
}

#define STB_TEXTEDIT_CHARTYPE					char32_t
#define STB_TEXTEDIT_STRING						std::basic_string<STB_TEXTEDIT_CHARTYPE>

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



namespace oe::gui
{
	float timer_key_pressed = 0;
	void resetTimer()
	{
		auto& clock = oe::utils::Clock::getSingleton();
		timer_key_pressed = clock.getSessionMillisecond() + 1000.0f;
	}
	
	TextInput::TextInput(Widget* parent, GUI& gui_manager, const TextInputInfo& _text_input_info)
		: Widget(parent, gui_manager, _text_input_info.widget_info)
		, text_input_info(_text_input_info)
		, m_selected(false)
		, m_filtering(_text_input_info.filter != filter_none)
	{
		m_state = new STB_TexteditState();
		static_cast<STB_TexteditState*>(m_state)->cursor = static_cast<int>(text_input_info.text.size());
	}

	TextInput::~TextInput()
	{
		delete static_cast<STB_TexteditState*>(m_state);
	}
	
	void TextInput::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			quad = m_gui_manager.getRenderer()->create();
			text_quad = m_gui_manager.getLateRenderer()->create();
			text_bar_quad = m_gui_manager.getLateRenderer()->create();
			label = new oe::graphics::u32TextLabel(m_gui_manager.getFont(text_input_info.font_size, text_input_info.font_path));

			// event listeners
			std::lock_guard(m_gui_manager.getWindow()->dispatcher_mutex);
			m_cg_render = { m_gui_manager.dispatcher, this };
			m_cg_codepoint = { m_gui_manager.getWindow()->getGameloop().getDispatcher(), this };
			m_cg_key = { m_gui_manager.getWindow()->getGameloop().getDispatcher(), this };
			m_cg_button = { m_gui_manager.getWindow()->getGameloop().getDispatcher(), this };
		}
		else
		{
			quad.reset();
			text_quad.reset();
			text_bar_quad.reset();
			delete label;

			// event listeners
			m_cg_render.reset();
			m_cg_codepoint.reset();
			m_cg_key.reset();
			m_cg_button.reset();
		}
	}
	
	void TextInput::on_render(const GUIRenderEvent& event)
	{
		// bounding box
		quad->setPosition(render_position);
		quad->setZ(z);
		quad->setSize(static_cast<glm::vec2>(m_info.size));
		quad->setSprite(text_input_info.sprite);
		quad->setColor(text_input_info.color);
		quad->update();

		// text
		const std::u32string& drawn_str = text_input_info.text;
		label->generate({ drawn_str, text_input_info.default_text_color }, m_gui_manager.getWindow());
		const glm::ivec2 text_label_pos = render_position + oe::alignmentOffset(m_info.size, text_input_info.align_text) - oe::alignmentOffset(static_cast<glm::ivec2>(label->getSize()), text_input_info.align_text);
		
		// text label
		text_quad->setPosition(static_cast<glm::vec2>(text_label_pos));
		text_quad->setZ(z + 0.025f);
		text_quad->setSize(label->getSize());
		text_quad->setSprite(label->getSprite());
		text_quad->setColor(oe::colors::white);
		text_quad->update();
		
		// text bar
		auto& clock = oe::utils::Clock::getSingleton();
		float time = clock.getSessionMillisecond();
		bool bar = m_selected && (timer_key_pressed > clock.getSessionMillisecond() || (int)floor(time) % 1000 > 500);
		auto& font = m_gui_manager.getFont(text_input_info.font_size, text_input_info.font_path);
		const glm::ivec2 before_cursor_size = oe::graphics::u32Text::size(font, drawn_str.substr(0, static_cast<STB_TexteditState*>(m_state)->cursor), glm::vec2(font.getResolution()));
		text_bar_quad->setPosition(static_cast<glm::vec2>(text_label_pos + glm::ivec2{ before_cursor_size.x, 0 }));
		text_bar_quad->setZ(z + 0.05f);
		text_bar_quad->setSize({ 1, font.getResolution() });
		text_bar_quad->setSprite(text_input_info.sprite);
		text_bar_quad->setColor(text_input_info.default_text_color);
		text_bar_quad->toggle(bar);
		text_bar_quad->update();
	}

	void TextInput::on_codepoint(const CodepointEvent& event)
	{
		if (!m_selected) return;
		char32_t character = event.codepoint;

		if (m_filtering) {
			if (text_input_info.filter.find(character) == text_input_info.filter.npos)
				return;
		}

		stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), character);
		resetTimer();

		TextInputChangedEvent e;
		e.character = character;
		e.value = text_input_info.text;
	}
	
	void TextInput::on_key(const KeyboardEvent& event)
	{
		if (!m_selected) return;

		if (event.action != oe::actions::release)
		{
			if (event.key == oe::keys::key_backspace)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_BACKSPACE);
			else if (event.key == oe::keys::key_delete)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_DELETE);
			else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::none)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_LEFT);
			else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::control)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDLEFT);
			else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::none)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_RIGHT);
			else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::control)
				stb_textedit_key(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDRIGHT);
			else if (event.key == oe::keys::key_v && event.mods == oe::modifiers::control) { // ctrl + v
				std::u32string cb = oe::utils::convertUTF<std::string, std::u32string>(m_gui_manager.getWindow()->getClipboard());
				stb_textedit_paste(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state), cb.c_str(), static_cast<int>(cb.size()));
			}
			else if (event.key == oe::keys::key_c && event.mods == oe::modifiers::control) { // ctrl + c
				std::u32string copied = text_input_info.text.substr(reinterpret_cast<STB_TexteditState*>(m_state)->select_start, reinterpret_cast<STB_TexteditState*>(m_state)->select_end - reinterpret_cast<STB_TexteditState*>(m_state)->select_start);
				m_gui_manager.getWindow()->setClipboard(oe::utils::convertUTF<std::u32string, std::string>(copied));
				stb_textedit_cut(&text_input_info.text, reinterpret_cast<STB_TexteditState*>(m_state));
			}
			else if (event.key == oe::keys::key_enter) {
				TextInputChangedEvent e;
				e.character = U'\n';
				e.value = text_input_info.text;
			}

			resetTimer();

			TextInputChangedEvent e;
			e.character = 0;
			e.value = text_input_info.text;
		}
	}
	
	void TextInput::on_button(const MouseButtonEvent& event)
	{
		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::press) {
			if (event.cursor_pos.cursor_windowspace.x >= render_position.x &&
				event.cursor_pos.cursor_windowspace.x < render_position.x + m_info.size.x &&
				event.cursor_pos.cursor_windowspace.y >= render_position.y &&
				event.cursor_pos.cursor_windowspace.y < render_position.y + m_info.size.y
			/*if (*/ )
			{
				m_selected = true;
				resetTimer();
			}
			else
			{
				m_selected = false;
			}
		}
	}

}