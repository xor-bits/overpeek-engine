#include "color_input.hpp"

#include "number_input.hpp"
#include "slider_input.hpp"
#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "color_picker.hpp"
#include "button.hpp"
#include "vec_slider.hpp"

#include "../gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui
{
	constexpr size_t vec_size = 4;

	constexpr std::array<char, 4> channel_names = {{
		'R',
		'G',
		'B',
		'A',
	}};

	constexpr std::array<std::string_view, 4> channel_full_names = {{
		"Red",
		"Green",
		"Blue",
		"Alpha",
	}};

	Vec<BasicSliderInput<float>, vec_size>::info_t create_vec_slider_info(const ColorInput::info_t& color_input_info)
	{
		Vec<BasicSliderInput<float>, vec_size>::info_t info;
		info.type = arrangements::rows;
		info.borders = 4;
		info.padding = 4;

		info.widget_info = color_input_info.widget_info;
		info.widget_info.pixel_size = { -48, 0 };
		info.widget_info.fract_size = { 1.0f, 1.0f };
		info.widget_info.pixel_origon_offset = { 0, 0 };
		info.widget_info.fract_origon_offset = oe::alignments::top_left;
		info.widget_info.fract_render_offset = oe::alignments::top_left;

		BasicSliderInput<float>::info_t common;
		common.value_bounds = glm::vec2(0.0f, 1.0f);
		common.text_options = color_input_info.text_options;
		common.slider_sprite = color_input_info.sprite;
		common.knob_sprite = color_input_info.sprite;

		info.common.fill(common);

		info.common[0].slider_lcolor = oe::colors::red;
		info.common[1].slider_lcolor = oe::colors::green;
		info.common[2].slider_lcolor = oe::colors::blue;
		info.common[3].slider_lcolor = oe::colors::black;

		for (size_t i = 0; i < vec_size; i++)
			info.common[i].text_format = [i, text_format = color_input_info.text_format](const float& val) { return oe::utils::convertUTF<std::u32string>(text_format(channel_names[i], channel_full_names[i], val)); };
		

		return info;
	}

	Vec<NumberInput, vec_size>::info_t create_vec_number_info(const ColorInput::info_t& color_input_info)
	{
		Vec<NumberInput, vec_size>::info_t info;
		info.type = arrangements::columns;
		info.borders = 4;
		info.padding = 4;

		info.widget_info = color_input_info.widget_info;
		info.widget_info.pixel_size = { -48, 0 };
		info.widget_info.fract_size = { 1.0f, 1.0f };
		info.widget_info.pixel_origon_offset = { 0, 0 };
		info.widget_info.fract_origon_offset = oe::alignments::top_left;
		info.widget_info.fract_render_offset = oe::alignments::top_left;

		NumberInput::info_t common;
		common.value_bounds = glm::vec2(0.0f, 1.0f);
		common.sprite = color_input_info.sprite;
		common.interact_flags = interact_type_flags::cursor | interact_type_flags::scroll;
		common.stepsize = 1.0f / 256.0f;
		common.text_options = color_input_info.text_options;
		common.background_color = oe::colors::darker_grey;

		info.common.fill(common);

		info.common[0].background_color = oe::colors::red;
		info.common[1].background_color = oe::colors::green;
		info.common[2].background_color = oe::colors::blue;
		info.common[3].background_color = oe::colors::black;

		for (size_t i = 0; i < vec_size; i++)
			info.common[i].text_format = [i, text_format = color_input_info.text_format](const float& val) { return text_format(channel_names[i], channel_full_names[i], val); };

		return info;
	}

	ColorInput::ColorInput(Widget* parent, GUI& gui_manager, const info_t& color_input_info, value_t& value_ref)
		: Widget(parent, gui_manager, color_input_info.widget_info)
		, m_color_input_info(color_input_info)
		, m_value(value_ref)
	{
		auto use_event = [this](const BasicSliderInputUseEvent<float>& e){
			m_event_use_latest.action = e.action;
			m_event_use_latest.button = e.button;
			m_event_use_latest.modifier = e.modifier;
			m_event_use_latest.value = m_value;
			m_dispatcher.trigger(m_event_use_latest);
		};

		auto hover_event = [this](const BasicSliderInputHoverEvent<float>&){
			m_dispatcher.trigger(m_event_hover_latest);
		};

		m_info.min_pixel_size = { 58, 8 };
		if(m_color_input_info.primary_input == input_type::slider)
		{
			const auto vec_info = create_vec_slider_info(color_input_info);
			auto vec = create(vec_info, m_value);
			
			for (size_t i = 0; i < vec_size; i++)
			{
				const int32_t min_size = std::min(vec->m_elements[i]->m_render_size.x, vec->m_elements[i]->m_render_size.y);
				vec->m_elements[i]->m_slider_info.knob_size = { min_size, min_size };
			}

			for (size_t i = 0; i < 4; i++)
			{
				m_cg_vec_use[i].connect<BasicSliderInputUseEvent<float>>(vec->m_elements[i]->m_dispatcher, use_event);
				m_cg_vec_hover[i].connect<BasicSliderInputHoverEvent<float>>(vec->m_elements[i]->m_dispatcher, hover_event);
			}
		}
		else if(m_color_input_info.primary_input == input_type::dragger)
		{
			const auto vec_info = create_vec_number_info(color_input_info);
			auto vec = Widget::create(vec_info, m_value);

			for (size_t i = 0; i < 4; i++)
			{
				m_cg_vec_use[i].connect<BasicSliderInputUseEvent<float>>(vec->m_elements[i]->m_dispatcher, use_event);
				m_cg_vec_hover[i].connect<BasicSliderInputHoverEvent<float>>(vec->m_elements[i]->m_dispatcher, hover_event);
			}
		}
		else
			m_info.min_pixel_size = { 48, 8 };
		

		SpritePanel::info_t sprite_panel_info;
		sprite_panel_info.widget_info.pixel_size = { 0, 0 };
		sprite_panel_info.widget_info.fract_size = { 1.0f, 1.0f };
		sprite_panel_info.sprite = m_color_input_info.sprite;
		sprite_panel_info.color_tint = m_color_input_info.background_color;
		auto bgbox = create(sprite_panel_info);
		bgbox->overrideZ(bgbox->getZ() - 0.5f);

		SpritePanel::info_t preview_panel_info;
		preview_panel_info.widget_info.pixel_size = { 40, -8 };
		preview_panel_info.widget_info.fract_size = { 0.0f, 1.0f };
		preview_panel_info.widget_info.pixel_origon_offset = { -4, 4 };
		preview_panel_info.widget_info.fract_origon_offset = oe::alignments::top_right;
		preview_panel_info.widget_info.fract_render_offset = oe::alignments::top_right;
		preview_panel_info.color_tint = m_value;
		preview_panel_info.sprite = m_color_input_info.sprite;
		m_preview_panel = create(preview_panel_info);

		if (m_color_input_info.popup_color_picker)
		{
			ColorPicker::info_t cpw_info;
			cpw_info.alpha = true;
			cpw_info.preview = true;
			cpw_info.color_input_info.background_color = m_color_input_info.background_color;
			cpw_info.color_input_info.sprite = m_color_input_info.sprite;
			cpw_info.color_input_info.widget_info.pixel_size = { 200, 200 };
			cpw_info.color_input_info.widget_info.pixel_origon_offset = static_cast<glm::ivec2>(m_gui_manager.getWindow()->getSize()) * 4;
			cpw_info.color_input_info.widget_info.toggled = true;
			m_popup_picker = create(cpw_info, m_value);
			m_popup_picker->addZ(1e2f);
			
			m_popup_picker->connect_listener<ColorPickerHoverEvent, &ColorInput::on_color_picker_hover>(this);
			m_popup_picker->connect_listener<ColorPickerUseEvent, &ColorInput::on_color_picker_use>(this);

			Button::info_t b_info;
			static_cast<Widget::info_t&>(b_info) = preview_panel_info.widget_info;
			m_preview_button = create(b_info);

			m_preview_button->connect_listener<ButtonHoverEvent, &ColorInput::on_button_hover>(this);
			m_preview_button->connect_listener<ButtonUseEvent, &ColorInput::on_button_use>(this);
		}

		m_value_last = m_value + 1.0f;
	}

	ColorInput::~ColorInput()
	{
		if (m_color_input_info.popup_color_picker)
		{
			m_popup_picker->disconnect_listener<ColorPickerHoverEvent, &ColorInput::on_color_picker_hover>(this);
			m_popup_picker->disconnect_listener<ColorPickerUseEvent, &ColorInput::on_color_picker_use>(this);

			m_preview_button->disconnect_listener<ButtonHoverEvent, &ColorInput::on_button_hover>(this);
			m_preview_button->disconnect_listener<ButtonUseEvent, &ColorInput::on_button_use>(this);
		}
	}
	
	void ColorInput::virtual_toggle(bool enabled)
	{
		Widget::virtual_toggle(enabled);
		if(enabled)
		{
			m_cg_render.connect<GUIRenderEvent, &ColorInput::on_render>(m_gui_manager.m_dispatcher, this);
			m_cg_mouse_button.connect<MouseButtonEvent, &ColorInput::on_mouse_button>(m_gui_manager.m_dispatcher, this);
			m_cg_cursor_pos.connect<CursorPosEvent, &ColorInput::on_cursor_pos>(m_gui_manager.m_dispatcher, this);
		}
		else
		{
			m_cg_render.disconnect();
			m_cg_mouse_button.disconnect();
			m_cg_cursor_pos.disconnect();
		}
	}

	void ColorInput::update()
	{
		m_value_last = m_value;
		m_preview_panel->sprite_panel_info.color_tint = m_value;
	}
	
	void ColorInput::on_render(const GUIRenderEvent& /* event */)
	{
		if(m_value_last != m_value)
			update();
	}
	
	void ColorInput::on_color_picker_hover(const ColorPickerHoverEvent& /* e */)
	{

	}

	void ColorInput::on_color_picker_use(const ColorPickerUseEvent& e)
	{
		m_value = e.value;
		
		m_event_use_latest.action = e.action;
		m_event_use_latest.button = e.button;
		m_event_use_latest.modifier = e.modifier;
		m_event_use_latest.value = e.value;
		m_dispatcher.trigger(m_event_use_latest);
	}

	void ColorInput::on_button_hover(const ButtonHoverEvent& /* e */)
	{
		if (m_color_input_info.popup_open != open_fn::in_bbox)
			return;

		m_popup_picker->m_info.pixel_origon_offset = m_gui_manager.getWindow()->getCursorWindow() - m_render_position + glm::ivec2{ 10, 10 };
		m_popup_picker->toggle(true);
	}

	void ColorInput::on_button_use(const ButtonUseEvent& e)
	{
		if (m_color_input_info.popup_open != open_fn::click_in)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		m_popup_picker->m_info.pixel_origon_offset = m_gui_manager.getWindow()->getCursorWindow() - m_render_position + glm::ivec2{ 10, 10 };
		m_popup_picker->toggle(true);
	}

	void ColorInput::on_mouse_button(const MouseButtonEvent& e)
	{
		if (m_color_input_info.popup_close != close_fn::click_out || !m_cg_mouse_button)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		if (!oe::utils::bounding_box_test(
			e.cursor_pos.cursor_windowspace,
			glm::min(m_preview_button->m_render_position, m_popup_picker->m_render_position),
			glm::max(m_preview_button->m_render_position + m_preview_button->m_render_size, m_popup_picker->m_render_position + m_popup_picker->m_render_size) - glm::min(m_preview_button->m_render_position, m_popup_picker->m_render_position)))
		{
			m_popup_picker->toggle(false);
		}
	}

	void ColorInput::on_cursor_pos(const CursorPosEvent& e)
	{
		if (m_color_input_info.popup_close != close_fn::leave_bbox || !m_cg_cursor_pos)
			return;

		constexpr int bbox_padding = 30;
		if (!oe::utils::bounding_box_test(
			e.cursor_windowspace,
			glm::min(m_preview_button->m_render_position, m_popup_picker->m_render_position) - glm::ivec2{ bbox_padding },
			glm::max(m_preview_button->m_render_position + m_preview_button->m_render_size, m_popup_picker->m_render_position + m_popup_picker->m_render_size) - glm::min(m_preview_button->m_render_position, m_popup_picker->m_render_position) + glm::ivec2{ bbox_padding * 2 }))
		{
			m_popup_picker->toggle(false);
		}
	}
}