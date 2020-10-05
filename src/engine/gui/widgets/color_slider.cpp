#include "color_slider.hpp"

#include "slider_input.hpp"
#include "sprite_panel.hpp"
#include "engine/engine.hpp"
#include "color_picker.hpp"
#include "button.hpp"

#include "../gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui
{
	VecSliderInfo<4> create_info(const ColorSliderInfo& color_slider_info)
	{
		const int slider_height = color_slider_info.widget_info.size.y;
		const int slider_width = color_slider_info.widget_info.size.x - 55;

		VecSliderInfo<4> info;
		info.type = arrangements::rows;
		info.initial_values = color_slider_info.initial_color;
		info.min_values = glm::vec4(0.0f);
		info.max_values = glm::vec4(1.0f);
		
		info.slider_info.widget_info = color_slider_info.widget_info;
		info.slider_info.widget_info.size = { slider_width, slider_height };
		info.slider_info.widget_info.offset_position += glm::ivec2{ 5, 5 };
		
		if(color_slider_info.draw_value == 2)
			info.slider_info.draw_format = [](const float& val) { return fmt::format(U"{}", static_cast<int>(val * 256.0f)); };

		info.slider_info.draw_value = color_slider_info.draw_value != 0;
		info.slider_info.slider_sprite = color_slider_info.sprite;
		info.slider_info.knob_sprite = color_slider_info.sprite;
		info.slider_info.draw_value = true;
		info.slider_borders = 5;

		return info;
	}

	ColorSlider::ColorSlider(Widget* parent, GUI& gui_manager, glm::vec4& value_ref, const ColorSliderInfo& color_slider_info)
		: VecSlider<4>(parent, gui_manager, value_ref, create_info(color_slider_info))
		, m_color_slider_info(color_slider_info)
		, m_value(value_ref)
	{
		SpritePanelInfo sprite_panel_info;
		sprite_panel_info.widget_info = { m_info.size + glm::ivec2{ 55, 10 }, { -5, -5 }, oe::alignments::top_left, oe::alignments::top_left };
		sprite_panel_info.sprite = m_color_slider_info.sprite;
		sprite_panel_info.color = m_color_slider_info.background_color;
		auto bgbox = create<SpritePanel>(sprite_panel_info);
		bgbox->overrideZ(bgbox->getZ() - 0.5f);

		SpritePanelInfo preview_panel_info;
		preview_panel_info.widget_info = { { 40, m_info.size.y }, { -5, 5 }, oe::alignments::top_right, oe::alignments::top_right };
		preview_panel_info.color = m_value;
		preview_panel_info.sprite = m_color_slider_info.sprite;
		m_preview_panel = bgbox->create<SpritePanel>(preview_panel_info);

		m_sliders[0]->m_slider_info.slider_lcolor = oe::colors::red;
		m_sliders[1]->m_slider_info.slider_lcolor = oe::colors::green;
		m_sliders[2]->m_slider_info.slider_lcolor = oe::colors::blue;
		m_sliders[3]->m_slider_info.slider_lcolor = oe::colors::black;
		
		if (m_color_slider_info.popup_color_picker)
		{
			ColorPickerInfo cpw_info;
			cpw_info.alpha = true;
			cpw_info.preview = true;
			cpw_info.color_slider_info.background_color = m_color_slider_info.background_color;
			cpw_info.color_slider_info.sprite = m_color_slider_info.sprite;
			cpw_info.color_slider_info.widget_info.size = { 200, 200 };
			cpw_info.color_slider_info.widget_info.align_parent = oe::alignments::top_left;
			cpw_info.color_slider_info.widget_info.align_render = oe::alignments::top_left;
			cpw_info.color_slider_info.widget_info.offset_position = m_gui_manager.getWindow()->getSize() * 4;
			cpw_info.color_slider_info.widget_info.toggled = true;
			m_popup_picker = create<ColorPicker>(m_value, cpw_info);
			
			m_popup_picker->connect_listener<ColorPickerHoverEvent, &ColorSlider::on_color_picker_hover>(this);
			m_popup_picker->connect_listener<ColorPickerUseEvent, &ColorSlider::on_color_picker_use>(this);

			ButtonInfo b_info;
			b_info.widget_info = preview_panel_info.widget_info;
			m_preview_button = bgbox->create<Button>(b_info);
			m_preview_button->connect_listener<ButtonHoverEvent, &ColorSlider::on_button_hover>(this);
			m_preview_button->connect_listener<ButtonUseEvent, &ColorSlider::on_button_use>(this);
		}

		m_value_last = m_value + 1.0f;

		connect_listener<VecSliderHoverEvent<4>, &ColorSlider::on_vec_slider_hover>(this);
		connect_listener<VecSliderUseEvent<4>, &ColorSlider::on_vec_slider_use>(this);
	}

	ColorSlider::~ColorSlider()
	{
		if (m_color_slider_info.popup_color_picker)
		{
			m_popup_picker->disconnect_listener<ColorPickerHoverEvent, &ColorSlider::on_color_picker_hover>(this);
			m_popup_picker->disconnect_listener<ColorPickerUseEvent, &ColorSlider::on_color_picker_use>(this);

			m_preview_button->disconnect_listener<ButtonHoverEvent, &ColorSlider::on_button_hover>(this);
			m_preview_button->disconnect_listener<ButtonUseEvent, &ColorSlider::on_button_use>(this);
		}

		disconnect_listener<VecSliderHoverEvent<4>, &ColorSlider::on_vec_slider_hover>(this);
		disconnect_listener<VecSliderUseEvent<4>, &ColorSlider::on_vec_slider_use>(this);
	}
	
	void ColorSlider::virtual_toggle(bool enabled)
	{
		VecSlider<4>::virtual_toggle(enabled);
		if(enabled)
		{
			m_cg_render.connect<GUIRenderEvent, &ColorSlider::on_render>(m_gui_manager.dispatcher, this);
			m_cg_mouse_button.connect<MouseButtonEvent, &ColorSlider::on_mouse_button>(m_gui_manager.dispatcher, this);
			m_cg_cursor_pos.connect<CursorPosEvent, &ColorSlider::on_cursor_pos>(m_gui_manager.dispatcher, this);
		}
		else
		{
			m_cg_render.disconnect();
			m_cg_mouse_button.disconnect();
			m_cg_cursor_pos.disconnect();
		}
	}

	void ColorSlider::update()
	{
		m_value_last = m_value;
		m_preview_panel->sprite_panel_info.color = m_value;
	}
	
	void ColorSlider::on_render(const GUIRenderEvent& event)
	{
		if(m_value_last != m_value)
			update();
	}
	
	void ColorSlider::on_color_picker_hover(const ColorPickerHoverEvent& e)
	{

	}

	void ColorSlider::on_color_picker_use(const ColorPickerUseEvent& e)
	{
		m_value = e.value;
		m_event_use_latest = e;
		dispatcher.trigger(m_event_use_latest);
	}

	void ColorSlider::on_vec_slider_hover(const VecSliderHoverEvent<4>& e)
	{
		dispatcher.trigger(m_event_hover_latest);
	}

	void ColorSlider::on_vec_slider_use(const VecSliderUseEvent<4>& e)
	{
		m_value = e.value;

		m_event_use_latest.action = e.action;
		m_event_use_latest.button = e.button;
		m_event_use_latest.modifier = e.modifier;
		m_event_use_latest.value = m_value;
		dispatcher.trigger(m_event_use_latest);
	}

	void ColorSlider::on_button_hover(const ButtonHoverEvent& e)
	{
		if (m_color_slider_info.popup_open != open_fn::in_bbox)
			return;

		m_popup_picker->m_info.offset_position = m_gui_manager.getWindow()->getCursorWindow() - render_position + glm::ivec2{ 10, 10 };
		m_popup_picker->toggle(true);
	}

	void ColorSlider::on_button_use(const ButtonUseEvent& e)
	{
		if (m_color_slider_info.popup_open != open_fn::click_in)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		m_popup_picker->m_info.offset_position = m_gui_manager.getWindow()->getCursorWindow() - render_position + glm::ivec2{ 10, 10 };
		m_popup_picker->toggle(true);
	}

	void ColorSlider::on_mouse_button(const MouseButtonEvent& e)
	{
		if (m_color_slider_info.popup_close != close_fn::click_out || !m_cg_mouse_button)
			return;

		if (e.button != oe::mouse_buttons::button_left || e.action != oe::actions::press)
			return;

		if (!oe::utils::bounding_box_test(
			e.cursor_pos.cursor_windowspace,
			glm::min(m_preview_button->render_position, m_popup_picker->render_position),
			glm::max(m_preview_button->render_position + m_preview_button->m_info.size, m_popup_picker->render_position + m_popup_picker->m_info.size) - glm::min(m_preview_button->render_position, m_popup_picker->render_position)))
		{
			m_popup_picker->toggle(false);
		}
	}

	void ColorSlider::on_cursor_pos(const CursorPosEvent& e)
	{
		if (m_color_slider_info.popup_close != close_fn::leave_bbox || !m_cg_cursor_pos)
			return;

		constexpr int bbox_padding = 30;
		if (!oe::utils::bounding_box_test(
			e.cursor_windowspace,
			glm::min(m_preview_button->render_position, m_popup_picker->render_position) - glm::ivec2{ bbox_padding },
			glm::max(m_preview_button->render_position + m_preview_button->m_info.size, m_popup_picker->render_position + m_popup_picker->m_info.size) - glm::min(m_preview_button->render_position, m_popup_picker->render_position) + glm::ivec2{ bbox_padding * 2 }))
		{
			m_popup_picker->toggle(false);
		}
	}
}