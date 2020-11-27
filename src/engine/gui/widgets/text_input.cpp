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
	oe::utils::text_flags state_flags(const typename BasicTextInput<char_type>::info_t& text_input_info)
	{
		oe::utils::text_flags flags = oe::utils::text_flags::none;
		if(text_input_info.allow_newline)
			flags = flags | oe::utils::text_flags::allow_newline;
		return flags;
	}

	template<typename char_type>
	BasicTextInput<char_type>::BasicTextInput(Widget* parent, GUI& gui_manager, const info_t& text_input_info, std::basic_string<char_type>& m_value_ref)
		: Widget(parent, gui_manager, text_input_info.widget_info)
		, m_text_label_pos({ 0, 0 })
		, m_text_input_info(text_input_info)
		, m_value(m_value_ref)
		, m_selected(false)
		, m_state(m_value, m_cache, m_text_input_info.max_characters, state_flags<char_type>(text_input_info))
		, m_timer_key_pressed(std::chrono::high_resolution_clock::now())
	{
		m_state.m_copy_to_clipboard = [this](const std::string& cb){ m_gui_manager.getWindow()->setClipboard(cb); };
		m_state.m_paste_from_clipboard = [this](std::string& cb){ cb = m_gui_manager.getWindow()->getClipboard(); };

		cursor() = 0;
		std::get<0>(selection()) = 0;
		std::get<1>(selection()) = 0;
	}

	template<typename char_type>
	[[nodiscard]] int& BasicTextInput<char_type>::cursor() const noexcept
	{
		return m_state.cursor();
	}
	
	template<typename char_type>
	[[nodiscard]] const std::tuple<int&, int&>& BasicTextInput<char_type>::selection() const noexcept
	{
		return m_state.selection();
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			m_quad = m_gui_manager.getRenderer()->create();
			m_text_quad = m_gui_manager.getRenderer()->create();
			m_text_bar_quad = m_gui_manager.getRenderer()->create();
			m_text_selection_quads[0] = m_gui_manager.getRenderer()->create();
			m_text_selection_quads[1] = m_gui_manager.getRenderer()->create();
			m_text_selection_quads[2] = m_gui_manager.getRenderer()->create();
			m_label = new oe::graphics::BasicTextLabel<char_type>(m_gui_manager.getFont(m_text_input_info.text_options.font), m_text_input_info.text_options.size);
			
			m_text_selection_quads[0]->setZ(m_z + 0.05f);
			m_text_selection_quads[1]->setZ(m_z + 0.06f);
			m_text_selection_quads[2]->setZ(m_z + 0.07f);			
			m_text_selection_quads[0]->setSprite(m_text_input_info.sprite);
			m_text_selection_quads[1]->setSprite(m_text_input_info.sprite);
			m_text_selection_quads[2]->setSprite(m_text_input_info.sprite);
			m_text_selection_quads[0]->setColor(m_text_input_info.selection_color);
			m_text_selection_quads[1]->setColor(m_text_input_info.selection_color);
			m_text_selection_quads[2]->setColor(m_text_input_info.selection_color);

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &BasicTextInput::on_render, BasicTextInput>(m_gui_manager.m_dispatcher, this);
			m_cg_codepoint.connect<CodepointEvent, &BasicTextInput::on_codepoint, BasicTextInput>(m_gui_manager.m_dispatcher, this );
			m_cg_key.connect<KeyboardEvent, &BasicTextInput::on_key, BasicTextInput>(m_gui_manager.m_dispatcher, this);
			m_cg_cursor.connect<CursorPosEvent, &BasicTextInput::on_cursor, BasicTextInput>(m_gui_manager.m_dispatcher, this);
			m_cg_button.connect<MouseButtonEvent, &BasicTextInput::on_button, BasicTextInput>(m_gui_manager.m_dispatcher, this);
		}
		else
		{
			m_quad.reset();
			m_text_quad.reset();
			m_text_bar_quad.reset();
			m_text_selection_quads[0].reset();
			m_text_selection_quads[1].reset();
			m_text_selection_quads[2].reset();
			delete m_label;

			// event listeners
			m_cg_render.disconnect();
			m_cg_codepoint.disconnect();
			m_cg_key.disconnect();
			m_cg_cursor.disconnect();
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
		m_text_input_info.text_format(m_value);
	}

	template<typename char_type>
	bool BasicTextInput<char_type>::regen_cache()
	{
		if(m_value_old == m_value)
			return false;
		m_value_old = m_value;

		auto& font = m_gui_manager.getFont(m_text_input_info.text_options.font);
		const oe::utils::color_string<char_type> string_vec = { m_value, m_text_input_info.text_options.initial_text_color };
		oe::graphics::BasicText<char_type>::create_text_render_cache(m_cache, string_vec, font, { 0.0f, 0.0f }, glm::vec2(m_text_input_info.text_options.size), oe::alignments::top_left);
		return true;
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_render(const GUIRenderEvent& /* event */)
	{
		if(!m_cg_render)
			return;

		// bounding box
		m_quad->setPosition(m_render_position);
		m_quad->setZ(m_z);
		m_quad->setSize(static_cast<glm::vec2>(m_info.size));
		m_quad->setSprite(m_text_input_info.sprite);
		m_quad->setColor(m_text_input_info.background_color);

		// text
		const oe::utils::color_string<char_type> string_vec = { m_value, m_text_input_info.text_options.initial_text_color };
		m_label->generate(string_vec, m_text_input_info.text_options);
		m_text_label_pos = m_render_position + oe::alignmentOffset(m_info.size, m_text_input_info.text_options.align) - oe::alignmentOffset(static_cast<glm::ivec2>(m_label->getSize()), m_text_input_info.text_options.align);
		
		// text label
		m_text_quad->toggle(m_text_input_info.text_options.enabled);
		m_text_quad->setPosition(static_cast<glm::vec2>(m_text_label_pos));
		m_text_quad->setZ(m_z + 0.025f);
		m_text_quad->setSize(m_label->getSize());
		m_text_quad->setSprite(m_label->getSprite());
		m_text_quad->setColor(oe::colors::white);

		m_state.clamp();
		m_text_bar_quad->toggle(m_selected);
		m_text_selection_quads[0]->toggle(m_selected);
		m_text_selection_quads[1]->toggle(m_selected);
		m_text_selection_quads[2]->toggle(m_selected);
		if(!m_selected)
			return;

		auto& clock = oe::utils::Clock::getSingleton();
		float time = clock.getSessionMillisecond();
		bool bar = (m_timer_key_pressed + std::chrono::seconds(1) > std::chrono::high_resolution_clock::now() || (int)floor(time) % 1000 > 500);
		m_text_bar_quad->toggle(bar);
		
		// TODO: update following (till the end of this scope) only if the cursor was updated

		// regen if user updated the string
		if(!regen_cache() && (m_cursor_old == m_state.cursor()) && (m_selection_old == m_state.selection()))
			return;
		m_cursor_old = m_state.cursor();
		m_selection_old = m_state.selection();


		// text bar
		const glm::ivec2 before_cursor_size = static_cast<glm::ivec2>(oe::graphics::BasicText<char_type>::offset_to_char(m_cache, m_state.cursor()));
		m_text_bar_quad->setPosition(static_cast<glm::vec2>(m_text_label_pos + before_cursor_size));
		m_text_bar_quad->setZ(m_z + 0.05f);
		m_text_bar_quad->setSize({ 1, m_text_input_info.text_options.size });
		m_text_bar_quad->setSprite(m_text_input_info.sprite);
		m_text_bar_quad->setColor(m_text_input_info.text_options.initial_text_color);

		// text selection
		const glm::ivec2 selection = { std::min(std::get<0>(m_state.selection()), std::get<1>(m_state.selection())), std::max(std::get<0>(m_state.selection()), std::get<1>(m_state.selection())) };
		const size_t lines = std::accumulate(
			m_value.begin() + selection.x,
			m_value.begin() + selection.y,
			size_t(1),
			[](size_t so_far, char_type ch){ return so_far + static_cast<int>(ch == '\n'); });
		const size_t lines_before = std::accumulate(
			m_value.begin(),
			m_value.begin() + selection.x,
			size_t(0),
			[](size_t so_far, char_type ch){ return so_far + static_cast<int>(ch == '\n'); });

		switch (lines)
		{
		case 1:
			{
				m_text_selection_quads[1]->setSize({ 0.0f, 0.0f });
				m_text_selection_quads[2]->setSize({ 0.0f, 0.0f });
				
				const glm::ivec2 selection_start_pos = static_cast<glm::ivec2>(oe::graphics::BasicText<char_type>::offset_to_char(m_cache, selection.x));
				const glm::ivec2 selection_end_pos = static_cast<glm::ivec2>(oe::graphics::BasicText<char_type>::offset_to_char(m_cache, selection.y));

				m_text_selection_quads[0]->setPosition(static_cast<glm::vec2>(m_text_label_pos + selection_start_pos));
				m_text_selection_quads[0]->setSize({ selection_end_pos.x - selection_start_pos.x, m_text_input_info.text_options.size });
			}
			break;
		default: // 3 or more
			{
				m_text_selection_quads[2]->setSize({ 0.0f, 0.0f });
				
				m_text_selection_quads[1]->setPosition(static_cast<glm::vec2>(m_text_label_pos + glm::ivec2{ -m_text_input_info.text_options.size, (lines_before + 1) * m_text_input_info.text_options.size }));
				m_text_selection_quads[1]->setSize({ m_label->getSize().x + 2 * m_text_input_info.text_options.size, (lines - 2) *m_text_input_info.text_options.size });
			}
			[[fallthrough]];
		case 2:
			{
				if(lines == 2)
					m_text_selection_quads[1]->setSize({ 0.0f, 0.0f });

				const glm::ivec2 selection_a_start_pos = static_cast<glm::ivec2>(oe::graphics::BasicText<char_type>::offset_to_char(m_cache, selection.x));
				const glm::ivec2 selection_b_end_pos = static_cast<glm::ivec2>(oe::graphics::BasicText<char_type>::offset_to_char(m_cache, selection.y));

				m_text_selection_quads[0]->setPosition(static_cast<glm::vec2>(m_text_label_pos + glm::ivec2{ selection_a_start_pos.x, lines_before * m_text_input_info.text_options.size }));
				m_text_selection_quads[0]->setSize({ m_text_input_info.text_options.size + m_label->getSize().x - selection_a_start_pos.x, m_text_input_info.text_options.size });
				m_text_selection_quads[2]->setPosition(static_cast<glm::vec2>(m_text_label_pos + glm::ivec2{ -m_text_input_info.text_options.size, (lines_before + lines - 1) * m_text_input_info.text_options.size }));
				m_text_selection_quads[2]->setSize({ m_text_input_info.text_options.size + selection_b_end_pos.x, m_text_input_info.text_options.size });
			}
			break;
		}
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

		// spdlog::debug("on codepoint <{}>", (size_t)c);

		m_state.key(c);
		reformat();
		regen_cache();
		
		BasicTextInputInputEvent<char_type> e { event.codepoint, m_value };
		m_dispatcher.trigger(e);
		resetTimer();
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_key(const KeyboardEvent& event)
	{
		if (!m_selected || !m_cg_key || event.action == oe::actions::release)
			return;
		resetTimer();

		m_state.key(event.key, event.mods);
		reformat();
		regen_cache();

		char_type character = 0;
		BasicTextInputInputEvent<char_type> e { static_cast<char32_t>(character), m_value };
		m_dispatcher.trigger(e);
	}

	template<typename char_type>
	void BasicTextInput<char_type>::on_cursor(const CursorPosEvent& event)
	{
		if(!m_cg_cursor || !m_dragging)
			return;
		
		m_state.drag(event.cursor_windowspace - m_text_label_pos);
	}
	
	template<typename char_type>
	void BasicTextInput<char_type>::on_button(const MouseButtonEvent& event)
	{
		if(!m_cg_button)
			return;

		if (event.button == oe::mouse_buttons::button_left && event.action != oe::actions::release)
		{
			m_selected = oe::utils::bounding_box_test(event.cursor_pos.cursor_windowspace, m_render_position, m_info.size);
			if(!m_selected)
				return;
			
			m_dragging = true;
			if(event.mods == oe::modifiers::shift) 
				m_state.drag(event.cursor_pos.cursor_windowspace - m_text_label_pos);
			else
				m_state.click(event.cursor_pos.cursor_windowspace - m_text_label_pos);
				
			// double click
			auto since_last = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch() - m_timer_key_pressed.time_since_epoch());
			if(since_last < m_double_click_delay)
			{
				std::get<0>(selection()) = m_value.size();
				std::get<1>(selection()) = 0;
				m_state.clamp();
			}

			// 
			resetTimer();
		}
		else
		{
			m_dragging = false;
		}
		
	}
	
	template class BasicTextInput<char>;
	template class BasicTextInput<wchar_t>;
	template class BasicTextInput<char16_t>;
	template class BasicTextInput<char32_t>;

}