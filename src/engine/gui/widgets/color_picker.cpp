#include "color_picker.hpp"

#include "slider.hpp"
#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "color_picker_wheel.hpp"
#include "button.hpp"

#include "../gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"



namespace oe::gui
{
	VecSliderInfo<4> create_info(const ColorPickerInfo& color_picker_info)
	{
		const int slider_height = color_picker_info.widget_info.size.y;
		const int slider_width = color_picker_info.widget_info.size.x - 55;

		VecSliderInfo<4> info;
		info.type = arrangements::rows;
		info.initial_values = color_picker_info.initial_color;
		info.min_values = glm::vec4(0.0f);
		info.max_values = glm::vec4(1.0f);
		
		info.slider_info.widget_info = color_picker_info.widget_info;
		info.slider_info.widget_info.size = { slider_width, slider_height };
		info.slider_info.widget_info.offset_position += glm::ivec2{ 5, 5 };
		
		if(color_picker_info.draw_value == 2)
			info.slider_info.draw_format = [](const SliderInfo& info) { return fmt::format(U"{}", static_cast<int>(info.value_initial * 256.0f)); };

		info.slider_info.draw_value = color_picker_info.draw_value != 0;
		info.slider_info.slider_sprite = color_picker_info.sprite;
		info.slider_info.knob_sprite = color_picker_info.sprite;
		info.slider_info.draw_value = true;
		info.slider_borders = 5;

		return info;
	}

	ColorPicker::ColorPicker(Widget* parent, GUI& gui_manager, const ColorPickerInfo& _color_picker_info)
		: VecSlider<4>(parent, gui_manager, create_info(_color_picker_info))
		, m_color_picker_info(_color_picker_info)
	{
		SpritePanelInfo sprite_panel_info;
		sprite_panel_info.widget_info = { m_info.size + glm::ivec2{ 55, 10 }, { -5, -5 }, oe::alignments::top_left, oe::alignments::top_left };
		sprite_panel_info.sprite = m_color_picker_info.sprite;
		sprite_panel_info.color = m_color_picker_info.background_color;
		auto bgbox = create<SpritePanel>(sprite_panel_info);
		bgbox->overrideZ(bgbox->getZ() - 0.5f);

		SpritePanelInfo preview_panel_info;
		preview_panel_info.widget_info = { { 40, m_info.size.y }, { -5, 5 }, oe::alignments::top_right, oe::alignments::top_right };
		preview_panel_info.color = m_color_picker_info.initial_color;
		preview_panel_info.sprite = m_color_picker_info.sprite;
		preview_panel = bgbox->create<SpritePanel>(preview_panel_info);

		sliders[0]->slider_info.slider_lcolor = oe::colors::red;
		sliders[1]->slider_info.slider_lcolor = oe::colors::green;
		sliders[2]->slider_info.slider_lcolor = oe::colors::blue;
		sliders[3]->slider_info.slider_lcolor = oe::colors::black;
		
		if (m_color_picker_info.popup_color_picker_wheel)
		{
			ColorPickerWheelInfo cpw_info;
			cpw_info.alpha = true;
			cpw_info.preview = true;
			cpw_info.color_picker_info.background_color = m_color_picker_info.background_color;
			cpw_info.color_picker_info.initial_color = m_color_picker_info.initial_color;
			cpw_info.color_picker_info.sprite = m_color_picker_info.sprite;
			cpw_info.color_picker_info.widget_info.size = { 200, 200 };
			cpw_info.color_picker_info.widget_info.align_parent = oe::alignments::top_left;
			cpw_info.color_picker_info.widget_info.align_render = oe::alignments::top_left;
			cpw_info.color_picker_info.widget_info.offset_position = m_gui_manager.getWindow()->getSize() * 4;
			cpw_info.color_picker_info.widget_info.toggled = false;
			popup_wheel = create<ColorPickerWheel>(cpw_info);
			popup_wheel->connect_listener<ColorPickerUseEvent, &ColorPicker::on_color_wheel_use>(this);

			ButtonInfo b_info;
			b_info.widget_info = preview_panel_info.widget_info;
			preview_button = bgbox->create<Button>(b_info);
			preview_button->connect_listener<ButtonHoverEvent, &ColorPicker::on_button_hover>(this);
			preview_button->connect_listener<ButtonUseEvent, &ColorPicker::on_button_use>(this);
		}

		connect_listener<VecSliderHoverEvent<4>, &ColorPicker::on_vec_slider_hover>(this);
		connect_listener<VecSliderUseEvent<4>, &ColorPicker::on_vec_slider_use>(this);
	}

	ColorPicker::~ColorPicker()
	{
		if (m_color_picker_info.popup_color_picker_wheel)
		{
			popup_wheel->disconnect_listener<ColorPickerUseEvent, &ColorPicker::on_color_wheel_use>(this);
			preview_button->disconnect_listener<ButtonHoverEvent, &ColorPicker::on_button_hover>(this);
			preview_button->disconnect_listener<ButtonUseEvent, &ColorPicker::on_button_use>(this);
		}

		disconnect_listener<VecSliderHoverEvent<4>, &ColorPicker::on_vec_slider_hover>(this);
		disconnect_listener<VecSliderUseEvent<4>, &ColorPicker::on_vec_slider_use>(this);
	}
	
	void ColorPicker::virtual_toggle(bool enabled)
	{
		VecSlider<4>::virtual_toggle(enabled);
		if(enabled)
		{
			std::lock_guard(m_gui_manager.getWindow()->dispatcher_mutex);
			m_cg_mouse_button.connect<MouseButtonEvent, &ColorPicker::on_mouse_button, ColorPicker>(m_gui_manager.getWindow()->getGameloop().getDispatcher(), this);
			m_cg_cursor_pos.connect<CursorPosEvent, &ColorPicker::on_cursor_pos, ColorPicker>(m_gui_manager.getWindow()->getGameloop().getDispatcher(), this);
		}
		else
		{
			m_cg_mouse_button.disconnect();
			m_cg_cursor_pos.disconnect();
		}
	}

	void ColorPicker::on_color_wheel_use(const ColorPickerUseEvent& e)
	{
		set(e.value);
		m_event_use_latest = e;
		dispatcher.trigger(m_event_use_latest);
	}

	void ColorPicker::on_vec_slider_hover(const VecSliderHoverEvent<4>& e)
	{
		dispatcher.trigger(m_event_hover_latest);
	}

	void ColorPicker::on_vec_slider_use(const VecSliderUseEvent<4>& e)
	{
		m_color_picker_info.initial_color = e.value;
		update();

		m_event_use_latest.action = e.action;
		m_event_use_latest.button = e.button;
		m_event_use_latest.modifier = e.modifier;
		m_event_use_latest.value = m_color_picker_info.initial_color;
		dispatcher.trigger(m_event_use_latest);
	}

	void ColorPicker::on_button_hover(const ButtonHoverEvent& e)
	{
		if (m_color_picker_info.popup_open != open_fn::in_bbox)
			return;

		popup_wheel->m_info.offset_position = m_gui_manager.getWindow()->getCursorWindow() - render_position + glm::ivec2{ 10, 10 };
		popup_wheel->toggle(true);
	}

	void ColorPicker::on_button_use(const ButtonUseEvent& e)
	{
		if (m_color_picker_info.popup_open != open_fn::click_in)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		popup_wheel->m_info.offset_position = m_gui_manager.getWindow()->getCursorWindow() - render_position + glm::ivec2{ 10, 10 };
		popup_wheel->toggle(true);
	}

	void ColorPicker::on_mouse_button(const MouseButtonEvent& e)
	{
		if (m_color_picker_info.popup_close != close_fn::click_out || !m_cg_mouse_button)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		if (!oe::utils::bounding_box_test(
			e.cursor_pos.cursor_windowspace,
			glm::min(preview_button->render_position, popup_wheel->render_position),
			glm::max(preview_button->render_position + preview_button->m_info.size, popup_wheel->render_position + popup_wheel->m_info.size) - glm::min(preview_button->render_position, popup_wheel->render_position)))
		{
			popup_wheel->toggle(false);
		}
	}

	void ColorPicker::on_cursor_pos(const CursorPosEvent& e)
	{
		if (m_color_picker_info.popup_close != close_fn::leave_bbox || !m_cg_cursor_pos)
			return;

		constexpr int bbox_padding = 30;
		if (!oe::utils::bounding_box_test(
			e.cursor_windowspace,
			glm::min(preview_button->render_position, popup_wheel->render_position) - glm::ivec2{ bbox_padding },
			glm::max(preview_button->render_position + preview_button->m_info.size, popup_wheel->render_position + popup_wheel->m_info.size) - glm::min(preview_button->render_position, popup_wheel->render_position) + glm::ivec2{ bbox_padding * 2 }))
		{
			popup_wheel->toggle(false);
		}
	}
	
	const glm::vec4& ColorPicker::get() const
	{
		return m_color_picker_info.initial_color;
	}

	void ColorPicker::set(const glm::vec4& color)
	{
		if (m_color_picker_info.initial_color == color)
			return;
		m_color_picker_info.initial_color = color;

		std::array<float, 4> arr;
		oe::utils::vecToContainer<4, float>(color, arr);
		VecSlider<4>::set(arr);

		update();
	}

	void ColorPicker::update()
	{
		preview_panel->sprite_panel_info.color = m_color_picker_info.initial_color;
		if (m_color_picker_info.popup_color_picker_wheel)
			popup_wheel->set(m_color_picker_info.initial_color);
	}

}