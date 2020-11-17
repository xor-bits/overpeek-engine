#pragma once

#include "text_input.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/utility/extra.hpp"



namespace oe::gui
{
	template<typename T>
	class BasicNumberInput : public TextInput
	{
	public:
		using value_t = T;
		/* using hover_event_t = BasicNumberInputHoverEvent<T>;
		using use_event_t = BasicNumberInputUseEvent<T>; */
		struct info_t
		{
			using widget_t = BasicNumberInput;

			// value
			T                                        initial_value = { 0 };
			T                                             stepsize = { 1 };
			glm::vec<2,T>                             value_bounds = { std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
			// visuals
			std::function<std::string(const value_t&)> text_format = &default_formatter; // only when keyboard input is disabled
			TextOptions                               text_options = {};
			oe::color                             background_color = oe::colors::dark_grey;
			const oe::graphics::Sprite*                     sprite = nullptr;
			// io
			interact_type_flags                     interact_flags = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;
			// base
			Widget::info_t                             widget_info = {};
		
			//
			[[nodiscard]] static inline std::string default_formatter(const T& val)
			{
				if constexpr(std::is_floating_point_v<T>)
					return fmt::format("V: {:.1f}", val);
				else
					return fmt::format("V: {}", val);
			}
		};

	public:
		info_t m_number_input_info;
		value_t& m_value;
	
	private:
		bool m_dragging = false;
		value_t m_value_start = {};
		glm::ivec2 m_drag_start = { 0, 0 };

		[[nodiscard]] static auto pick_formatter(bool& first, bool& dot) noexcept
		{
			if constexpr(std::is_floating_point_v<T>)
				return [&first, &dot](char ch)
				{
					const bool remove =
						((first && !(std::isdigit(ch) || ch == '-' || ch == '+' || ch == '.')) ||
						(!first && !(std::isdigit(ch) || ch == '.'))) || (dot && ch == '.');
					
					dot = dot || ch == '.';
					first = false;
					
					return remove;
				};
			else if constexpr(std::is_integral_v<T> && std::is_signed_v<T>)
				return [&first](char ch)
				{
					const bool remove =
						 (first && !(std::isdigit(ch) || ch == '-' || ch == '+')) ||
						(!first && !(std::isdigit(ch)));
					
					first = false;
					return remove;
				};
			else if constexpr(std::is_integral_v<T> && std::is_unsigned_v<T>)
				return [](char ch)
				{
					return !std::isdigit(ch);
				};
			else
				static_assert("T is not a float nor a int");
		}
		
		[[nodiscard]] static TextInput::info_t getTextInputInfo(const info_t& number_input_info) noexcept
		{
			TextInput::info_t info;
			info.text_options = number_input_info.text_options;
			info.background_color = number_input_info.background_color;
			info.sprite = number_input_info.sprite;
			info.widget_info = number_input_info.widget_info;
			info.whitelist = "+-.0123456789";
			info.initial_value = fmt::format("{}", number_input_info.initial_value);
			info.text_format = [](std::string& value){
				bool first = true;
				bool dot = false;
				value.erase(std::remove_if(value.begin(), value.end(), pick_formatter(first, dot)), value.end());
				if(value.size() == 0 || (value.size() == 1 && value[0] == '.')) value += '0';
			};
			return info;
		}

	public:
		BasicNumberInput(Widget* parent, GUI& gui_manager, const info_t& number_input_info, value_t& value_ref)
			: TextInput(parent, gui_manager, getTextInputInfo(number_input_info))
			, m_number_input_info(number_input_info)
			, m_value(value_ref)
		{
			cg_text_change.connect<oe::gui::BasicTextInputInputEvent<char>, &BasicNumberInput::on_text_change>(m_dispatcher, this);
			cg_keyboard.connect<oe::KeyboardEvent, &BasicNumberInput::on_keyboard>(m_gui_manager.m_dispatcher, this);
			cg_cursor.connect<oe::CursorPosEvent, &BasicNumberInput::on_cursor>(m_gui_manager.m_dispatcher, this);
			cg_button.connect<oe::MouseButtonEvent, &BasicNumberInput::on_button>(m_gui_manager.m_dispatcher, this);
			cg_scroll.connect<oe::ScrollEvent, &BasicNumberInput::on_scroll>(m_gui_manager.m_dispatcher, this);
			cg_pre_render.connect<oe::gui::GUIPreRenderEvent, &BasicNumberInput::on_pre_render>(m_gui_manager.m_dispatcher, this);
		}
		BasicNumberInput(Widget* parent, GUI& gui_manager, const info_t& number_input_info)
			: BasicNumberInput(parent, gui_manager, number_input_info, m_number_input_info.initial_value)
		{}

		void clamp() noexcept
		{
			m_value = std::clamp(m_value, m_number_input_info.value_bounds.x, m_number_input_info.value_bounds.y);
		}

	private:
		// events
		void on_pre_render(const oe::gui::GUIPreRenderEvent&)
		{
			if(!static_cast<bool>(m_number_input_info.interact_flags & interact_type_flags::keyboard))
			{
				TextInput::m_value = m_number_input_info.text_format(m_value);
				TextInput::m_selected = false;
			}
			else
				TextInput::m_value = fmt::format("{}", m_value);
		}
		void on_text_change(const oe::gui::BasicTextInputInputEvent<char>& e)
		{
			if(!static_cast<bool>(m_number_input_info.interact_flags & interact_type_flags::keyboard))
				return;
			
			try{
				m_value = oe::utils::stoT<T>(std::string(e.value));
			}catch(...){
				spdlog::warn("Parse error");
				m_value = {};
			}
			clamp();
		}
		void on_keyboard(const oe::KeyboardEvent& e)
		{
			if(e.action == oe::actions::release || !m_selected || !static_cast<bool>(m_number_input_info.interact_flags & interact_type_flags::keyboard))
				return;

			if(e.key == oe::keys::key_down)
				m_value -= m_number_input_info.stepsize;
			else if(e.key == oe::keys::key_up)
				m_value += m_number_input_info.stepsize;
			clamp();
			on_pre_render({});
		}
		void on_button(const oe::MouseButtonEvent& e)
		{
			if(e.action == oe::actions::release)
				m_dragging = false;
			
			if(e.action != oe::actions::press || !oe::utils::bounding_box_test(e.cursor_pos.cursor_windowspace, m_render_position, m_info.size) || !static_cast<bool>(m_number_input_info.interact_flags & interact_type_flags::cursor))
				return;

			m_dragging = true;
			m_drag_start = e.cursor_pos.cursor_windowspace;
			m_value_start = m_value;
		}
		void on_cursor(const oe::CursorPosEvent& e)
		{
			if(!m_dragging)
				return;
			
			const auto offset = static_cast<glm::fvec2>(e.cursor_windowspace - m_drag_start);

			m_value = m_value_start + offset.x * m_number_input_info.stepsize;
			clamp();
			on_pre_render({});
		}
		void on_scroll(const oe::ScrollEvent& e)
		{
			if(!m_selected || !static_cast<bool>(m_number_input_info.interact_flags & interact_type_flags::scroll))
				return;

			m_value += (e.scroll_delta.y - e.scroll_delta.x) * m_number_input_info.stepsize;
			clamp();
			on_pre_render({});
		}
		oe::utils::connect_guard cg_text_change;
		oe::utils::connect_guard cg_keyboard;
		oe::utils::connect_guard cg_button;
		oe::utils::connect_guard cg_cursor;
		oe::utils::connect_guard cg_scroll;
		oe::utils::connect_guard cg_pre_render;
	};

	using fNumberInput = BasicNumberInput<float>;
	using dNumberInput = BasicNumberInput<double>;
	using iNumberInput = BasicNumberInput<int32_t>;
	using uNumberInput = BasicNumberInput<uint32_t>;
	using NumberInput = fNumberInput;
}