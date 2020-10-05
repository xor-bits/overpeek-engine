#include "text_input.hpp"

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/font.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/utility/clock.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui
{
	template<typename char_type>
	TextInput<char_type>::TextInput(Widget* parent, GUI& gui_manager, std::basic_string<char_type>& m_value_ref, const TextInputInfo<char_type>& text_input_info)
		: __TextInputBase<char_type>(m_text_input_info.regex_filter)
		, Widget(parent, gui_manager, text_input_info.widget_info)
		, m_text_input_info(text_input_info)
		, m_value(m_value_ref)
		, m_state()
		, m_selected(false)
		, m_timer_key_pressed(std::chrono::high_resolution_clock::now)
	{
		m_state.cursor() = static_cast<int>(m_text_input_info.initial_value.size() - 1);
	}
	
	template<typename char_type>
	void TextInput<char_type>::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			m_quad = m_gui_manager.getRenderer()->create();
			m_text_quad = m_gui_manager.getRenderer()->create();
			m_text_bar_quad = m_gui_manager.getRenderer()->create();
			m_label = new oe::graphics::u32TextLabel(m_gui_manager.getFont(m_text_input_info.font_size, m_text_input_info.font_file));

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &TextInput::on_render, TextInput>(m_gui_manager.dispatcher, this);
			m_cg_codepoint.connect<CodepointEvent, &TextInput::on_codepoint, TextInput>(m_gui_manager.dispatcher, this );
			m_cg_key.connect<KeyboardEvent, &TextInput::on_key, TextInput>(m_gui_manager.dispatcher, this);
			m_cg_button.connect<MouseButtonEvent, &TextInput::on_button, TextInput>(m_gui_manager.dispatcher, this);
		}
		else
		{
			m_quad.reset();
			m_text_quad.reset();
			m_text_bar_quad.reset();
			delete m_label;

			// event listeners
			m_cg_render.disconnect();
			m_cg_codepoint.disconnect();
			m_cg_key.disconnect();
			m_cg_button.disconnect();
		}
	}

	template<typename char_type>
	void TextInput<char_type>::resetTimer() noexcept
	{
		timer_key_pressed = std::chrono::high_resolution_clock::now().time_since_epoch() + std::chrono::seconds(1);
	}
	
	template<typename char_type>
	void TextInput<char_type>::on_render(const GUIRenderEvent& event)
	{
		if(!m_cg_render)
			return;

		// bounding box
		m_quad->setPosition(render_position);
		m_quad->setZ(z);
		m_quad->setSize(static_cast<glm::vec2>(m_info.size));
		m_quad->setSprite(m_text_input_info.sprite);
		m_quad->setColor(m_text_input_info.color);

		// text
		m_label->generate({ m_value, m_text_input_info.default_text_color }, m_gui_manager.getWindow());
		const glm::ivec2 text_label_pos = render_position + oe::alignmentOffset(m_info.size, m_text_input_info.align_text) - oe::alignmentOffset(static_cast<glm::ivec2>(m_label->getSize()), m_text_input_info.align_text);
		
		// text label
		m_text_quad->setPosition(static_cast<glm::vec2>(text_label_pos));
		m_text_quad->setZ(z + 0.025f);
		m_text_quad->setSize(m_label->getSize());
		m_text_quad->setSprite(m_label->getSprite());
		m_text_quad->setColor(oe::colors::white);
		
		// text bar
		auto& clock = oe::utils::Clock::getSingleton();
		float time = clock.getSessionMillisecond();
		bool bar = m_selected && (timer_key_pressed > clock.getSessionMillisecond() || (int)floor(time) % 1000 > 500);
		auto& font = m_gui_manager.getFont(m_text_input_info.font_size, m_text_input_info.font_file);
		const glm::ivec2 before_cursor_size = oe::graphics::u32Text::size(font, m_value.substr(0, m_state.cursor()), glm::vec2(font.getResolution()));
		m_text_bar_quad->setPosition(static_cast<glm::vec2>(text_label_pos + glm::ivec2{ before_cursor_size.x, 0 }));
		m_text_bar_quad->setZ(z + 0.05f);
		m_text_bar_quad->setSize({ 1, font.getResolution() });
		m_text_bar_quad->setSprite(m_text_input_info.sprite);
		m_text_bar_quad->setColor(m_text_input_info.default_text_color);
		m_text_bar_quad->toggle(bar);
	}

	template<typename char_type>
	void TextInput<char_type>::on_codepoint(const CodepointEvent& event)
	{
		if (!m_selected || !m_cg_codepoint)
			return;
		char32_t character = event.codepoint;

		m_state.key(m_value, character);
		if (!std::regex_search(m_value, m_regex))
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_UNDO);
		
		TextInputChangedEvent e { character, m_value };
		dispatcher.trigger(e);
		resetTimer();
	}
	
	template<typename char_type>
	void TextInput<char_type>::on_key(const KeyboardEvent& event)
	{
		if (!m_selected || !m_cg_key || event.action == oe::actions::release)
			return;

		char32_t character = 0;

		if (event.key == oe::keys::key_backspace)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_BACKSPACE);
		else if (event.key == oe::keys::key_delete)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_DELETE);
		else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::none)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_LEFT);
		else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::control)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDLEFT);
		else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::none)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_RIGHT);
		else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::control)
			stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_WORDRIGHT);
		else if (event.key == oe::keys::key_v && event.mods == oe::modifiers::control) // ctrl + v
		{
			std::u32string cb = oe::utils::convertUTF<std::string, std::u32string>(m_gui_manager.getWindow()->getClipboard());
			stb_textedit_paste(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), cb.c_str(), static_cast<int>(cb.size()));
			if (!std::regex_search(m_value, m_regex))
				stb_textedit_key(&m_value, reinterpret_cast<STB_TexteditState*>(m_state), STB_TEXTEDIT_K_UNDO);
		}
		else if (event.key == oe::keys::key_c && event.mods == oe::modifiers::control) // ctrl + c
		{
			std::u32string copied = m_value.substr(reinterpret_cast<STB_TexteditState*>(m_state)->select_start, reinterpret_cast<STB_TexteditState*>(m_state)->select_end - reinterpret_cast<STB_TexteditState*>(m_state)->select_start);
			m_gui_manager.getWindow()->setClipboard(oe::utils::convertUTF<std::u32string, std::string>(copied));
			stb_textedit_cut(&m_value, reinterpret_cast<STB_TexteditState*>(m_state));
		}
		else if (event.key == oe::keys::key_enter && m_text_input_info.allow_newline)
		{
			character = U'\n';
			m_value += character;
		}

		TextInputChangedEvent e { character, m_value };
		dispatcher.trigger(e);
		resetTimer();
	}
	
	template<typename char_type>
	void TextInput<char_type>::on_button(const MouseButtonEvent& event)
	{
		if(!m_cg_button)
			return;

		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::press)
		{
			m_selected = oe::utils::bounding_box_test(event.cursor_pos.cursor_windowspace, render_position, m_info.size);
			resetTimer();
		}
	}

}