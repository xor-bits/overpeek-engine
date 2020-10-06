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
	BasicTextInput<char_type>::BasicTextInput(Widget* parent, GUI& gui_manager, std::basic_string<char_type>& m_value_ref, const BasicTextInputInfo<char_type>& text_input_info)
		: Widget(parent, gui_manager, text_input_info.widget_info)
		, m_text_input_info(text_input_info)
		, m_value(m_value_ref)
		, m_state()
		, m_selected(false)
		, m_timer_key_pressed(std::chrono::high_resolution_clock::now())
	{
		m_state.cursor() = 0;
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			m_quad = m_gui_manager.getRenderer()->create();
			m_text_quad = m_gui_manager.getRenderer()->create();
			m_text_bar_quad = m_gui_manager.getRenderer()->create();
			m_label = new oe::graphics::BasicTextLabel<char_type>(m_gui_manager.getFont(m_text_input_info.font_size, m_text_input_info.font_file));

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &BasicTextInput::on_render, BasicTextInput>(m_gui_manager.dispatcher, this);
			m_cg_codepoint.connect<CodepointEvent, &BasicTextInput::on_codepoint, BasicTextInput>(m_gui_manager.dispatcher, this );
			m_cg_key.connect<KeyboardEvent, &BasicTextInput::on_key, BasicTextInput>(m_gui_manager.dispatcher, this);
			m_cg_button.connect<MouseButtonEvent, &BasicTextInput::on_button, BasicTextInput>(m_gui_manager.dispatcher, this);
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
	void BasicTextInput<char_type>::resetTimer() noexcept
	{
		m_timer_key_pressed = std::chrono::high_resolution_clock::now();
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::reformat() noexcept
	{
		m_text_input_info.formatter(m_value);
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_render(const GUIRenderEvent& event)
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
		bool bar = m_selected && (m_timer_key_pressed + std::chrono::seconds(1) > std::chrono::high_resolution_clock::now() || (int)floor(time) % 1000 > 500);
		auto& font = m_gui_manager.getFont(m_text_input_info.font_size, m_text_input_info.font_file);
		const glm::ivec2 before_cursor_size = oe::graphics::BasicText<char_type>::size(font, m_value.substr(0, m_state.cursor()), glm::vec2(font.getResolution()));
		m_text_bar_quad->setPosition(static_cast<glm::vec2>(text_label_pos + glm::ivec2{ before_cursor_size.x, 0 }));
		m_text_bar_quad->setZ(z + 0.05f);
		m_text_bar_quad->setSize({ 1, font.getResolution() });
		m_text_bar_quad->setSprite(m_text_input_info.sprite);
		m_text_bar_quad->setColor(m_text_input_info.default_text_color);
		m_text_bar_quad->toggle(bar);
	}

	template<typename char_type>
	void BasicTextInput<char_type>::on_codepoint(const CodepointEvent& event)
	{
		if (!m_selected || !m_cg_codepoint)
			return;

		uint32_t ch = static_cast<uint32_t>(event.codepoint);
		if (ch > std::numeric_limits<std::make_unsigned_t<char_type>>::max())
			return;
		char_type c = static_cast<char_type>(event.codepoint);

		if((!m_text_input_info.whitelist.empty() && m_text_input_info.whitelist.find(c) == std::basic_string_view<char_type>::npos) ||
		   (!m_text_input_info.blacklist.empty() && m_text_input_info.blacklist.find(c) != std::basic_string_view<char_type>::npos))
			return;
		m_state.key(m_value, c);
		reformat();
		
		BasicTextInputChangedEvent<char_type> e { event.codepoint, m_value };
		dispatcher.trigger(e);
		resetTimer();
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_key(const KeyboardEvent& event)
	{
		if (!m_selected || !m_cg_key || event.action == oe::actions::release)
			return;

		char_type character = 0;

		if (event.key == oe::keys::key_backspace)
			m_state.key(m_value, oe::utils::text_keys::backspace);
		else if (event.key == oe::keys::key_delete)
			m_state.key(m_value, oe::utils::text_keys::del);
		else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::none)
			m_state.key(m_value, oe::utils::text_keys::left);
		else if (event.key == oe::keys::key_left && event.mods == oe::modifiers::control)
			m_state.key(m_value, oe::utils::text_keys::c_left);
		else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::none)
			m_state.key(m_value, oe::utils::text_keys::right);
		else if (event.key == oe::keys::key_right && event.mods == oe::modifiers::control)
			m_state.key(m_value, oe::utils::text_keys::c_right);
		else if (event.key == oe::keys::key_z && event.mods == oe::modifiers::control)
			m_state.key(m_value, oe::utils::text_keys::undo);
		else if (event.key == oe::keys::key_y && event.mods == oe::modifiers::control)
			m_state.key(m_value, oe::utils::text_keys::redo);
		else if (event.key == oe::keys::key_v && event.mods == oe::modifiers::control) // ctrl + v
		{
			auto cb = oe::utils::convertUTF<std::basic_string<char_type>>(m_gui_manager.getWindow()->getClipboard());
			m_state.paste(m_value, cb);
		}
		else if (event.key == oe::keys::key_c && event.mods == oe::modifiers::control) // ctrl + c
		{
			std::basic_string<char_type> cb;
			m_state.copy(m_value, cb);
			m_gui_manager.getWindow()->setClipboard(oe::utils::convertUTF<std::string>(cb));
		}
		else if (event.key == oe::keys::key_x && event.mods == oe::modifiers::control) // ctrl + c
		{
			std::basic_string<char_type> cb;
			m_state.cut(m_value, cb);
			m_gui_manager.getWindow()->setClipboard(oe::utils::convertUTF<std::string>(cb));
		}
		else if (event.key == oe::keys::key_enter)
			m_state.key(m_value, '\n');
		reformat();

		BasicTextInputChangedEvent<char_type> e { static_cast<char32_t>(character), m_value };
		dispatcher.trigger(e);
		resetTimer();
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_button(const MouseButtonEvent& event)
	{
		if(!m_cg_button)
			return;

		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::press)
		{
			m_selected = oe::utils::bounding_box_test(event.cursor_pos.cursor_windowspace, render_position, m_info.size);
			resetTimer();
		}
	}
	
	template class BasicTextInput<char>;
	template class BasicTextInput<wchar_t>;
	template class BasicTextInput<char16_t>;
	template class BasicTextInput<char32_t>;

}