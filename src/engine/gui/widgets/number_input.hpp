#pragma once

#include "text_input.hpp"



namespace oe::gui
{

	template<typename T>
	struct BasicNumberInputInfo
	{
		glm::vec2 align_text               = oe::alignments::center_center;
		T initial_value                    = { 0 };
		T stepsize                         = { 1 };
		glm::vec<2,T> value_bounds         = { std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
		uint16_t font_size                 = 16;
		oe::utils::FontFile font_file      = {}; // empty for gui default
		glm::vec4 color                    = oe::colors::dark_grey;
		glm::vec4 default_text_color       = oe::colors::black;
		const oe::graphics::Sprite* sprite = nullptr;

		interact_type_flags interact_flags = interact_type_flags::cursor | interact_type_flags::keyboard | interact_type_flags::scroll;

		WidgetInfo widget_info             = { { 100, 100 }, { 3, 3 }, oe::alignments::center_center, oe::alignments::center_center };
	};

	template<typename T>
	class BasicNumberInput : public TextInput
	{
	public:
		BasicNumberInputInfo<T> m_number_input_info;
	
	private:
		T& m_value;
		bool m_dragging = false;
		T m_value_start = {};
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
		
		[[nodiscard]] static TextInputInfo getTextInputInfo(const BasicNumberInputInfo<T>& number_input_info) noexcept
		{
			TextInputInfo info;
			info.align_text = number_input_info.align_text;
			info.font_size = number_input_info.font_size;
			info.font_file = number_input_info.font_file;
			info.color = number_input_info.color;
			info.default_text_color = number_input_info.default_text_color;
			info.sprite = number_input_info.sprite;
			info.widget_info = number_input_info.widget_info;
			info.whitelist = "+-.0123456789";
			info.initial_value = fmt::format("{}", number_input_info.initial_value);
			info.formatter = [](std::string& value){
				bool first = true;
				bool dot = false;
				value.erase(std::remove_if(value.begin(), value.end(), pick_formatter(first, dot)), value.end());
				if(value.size() == 0 || (value.size() == 1 && value[0] == '.')) value += '0';
			};
			return info;
		}

		// string to T
		template<class number>
		static number stoT(const std::string& str);
		
		template<> static int stoT(const std::string& str) { return std::stoi(str); }
		template<> static long stoT(const std::string& str) { return std::stol(str); }
		template<> static long long stoT(const std::string& str) { return std::stoll(str); }
		template<> static unsigned int stoT(const std::string& str) { return std::stoui(str); }
		template<> static unsigned long stoT(const std::string& str) { return std::stoul(str); }
		template<> static unsigned long long stoT(const std::string& str) { return std::stoull(str); }
		template<> static float stoT(const std::string& str) { return std::stof(str); }
		template<> static double stoT(const std::string& str) { return std::stod(str); }
		template<> static long double stoT(const std::string& str) { return std::stold(str); }

	public:
		BasicNumberInput(Widget* parent, GUI& gui_manager, T& value_ref, const BasicNumberInputInfo<T>& number_input_info = {})
			: TextInput(parent, gui_manager, getTextInputInfo(number_input_info))
			, m_number_input_info(number_input_info)
			, m_value(value_ref)
		{
			cg_keyboard.connect<oe::gui::TextInputChangedEvent, &BasicNumberInput::on_text_change>(dispatcher, this);
			cg_keyboard.connect<oe::KeyboardEvent, &BasicNumberInput::on_keyboard>(m_gui_manager.dispatcher, this);
			cg_keyboard.connect<oe::CursorPosEvent, &BasicNumberInput::on_cursor>(m_gui_manager.dispatcher, this);
			cg_keyboard.connect<oe::MouseButtonEvent, &BasicNumberInput::on_button>(m_gui_manager.dispatcher, this);
			cg_keyboard.connect<oe::ScrollEvent, &BasicNumberInput::on_scroll>(m_gui_manager.dispatcher, this);
			cg_keyboard.connect<oe::gui::GUIPreRenderEvent, &BasicNumberInput::on_pre_render>(m_gui_manager.dispatcher, this);
		}
		BasicNumberInput(Widget* parent, GUI& gui_manager, const BasicNumberInputInfo<T>& number_input_info = {}) : BasicNumberInput(parent, gui_manager, m_number_input_info.initial_value, number_input_info) {}

		void clamp() noexcept
		{
			m_value = std::clamp(m_value, m_number_input_info.value_bounds.x, m_number_input_info.value_bounds.y);
		}

	private:
		// events
		void on_pre_render(const oe::gui::GUIPreRenderEvent&)
		{
			TextInput::m_value = fmt::format("{}", m_value);
		}
		void on_text_change(const oe::gui::TextInputChangedEvent& e)
		{
			try{
				m_value = stoT<T>(std::string(e.value));
			}catch(...){
				spdlog::warn("Parse error");
				m_value = {};
			}
			clamp();
		}
		void on_keyboard(const oe::KeyboardEvent& e)
		{
			if(e.action == oe::actions::release || !(m_number_input_info.interact_flags & interact_type_flags::keyboard))
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
			
			if(e.action != oe::actions::press || !oe::utils::bounding_box_test(e.cursor_pos.cursor_windowspace, render_position, m_info.size) || !(m_number_input_info.interact_flags & interact_type_flags::cursor))
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
			if(!(m_number_input_info.interact_flags & interact_type_flags::scroll))
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

	};
}