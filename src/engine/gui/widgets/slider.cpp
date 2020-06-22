#include "slider.hpp"

#include "engine/engine.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {

	Slider::Slider(const SliderInfo& _slider_info) 
		: Widget(_slider_info.slider_size, _slider_info.align_parent, _slider_info.align_render, _slider_info.offset_position)
		, slider_info(_slider_info)
		, m_dragging(false)
	{
		if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		if (slider_info.draw_value)
		{
			value_label = new oe::graphics::TextLabel();
		}
	}

	Slider::~Slider()
	{
		if (slider_info.draw_value)
		{
			delete value_label;
		}
	}

	void Slider::managerAssigned(GUI* gui_manager)
	{
		if (slider_info.draw_value)
		{
			label_quad = gui_manager->getLateRenderer()->createQuad();
		}
		quad_knob = gui_manager->getRenderer()->createQuad();
		quad_lslider = gui_manager->getRenderer()->createQuad();
		quad_rslider = gui_manager->getRenderer()->createQuad();

		// event listeners
		gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		gui_manager->getWindow()->connect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		gui_manager->getWindow()->connect_listener<oe::ScrollEvent, &Slider::on_scroll>(this);
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Slider::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void Slider::managerUnassigned(GUI* gui_manager)
	{
		if (slider_info.draw_value)
		{
			gui_manager->getLateRenderer()->destroyQuad(label_quad);
		}
		gui_manager->getRenderer()->destroyQuad(quad_knob);
		gui_manager->getRenderer()->destroyQuad(quad_lslider);
		gui_manager->getRenderer()->destroyQuad(quad_rslider);

		// event listeners
		gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		gui_manager->getWindow()->disconnect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		gui_manager->getWindow()->disconnect_listener<oe::ScrollEvent, &Slider::on_scroll>(this);
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Slider::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}
	
	void Slider::on_render(const GUIRenderEvent& event)
	{
		glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.initial_value, slider_info.min_value, slider_info.max_value, 0.0f, static_cast<float>(size.x - slider_info.knob_size.x)), 0.0f);
		
		{
			*event.z += 1.0f;
			quad_lslider->setPosition(render_position);
			quad_lslider->setZ(*event.z);
			quad_lslider->setSize({ slider_pos.x, size.y });
			quad_lslider->setColor(slider_info.slider_lcolor);
			quad_lslider->setSprite(slider_info.slider_sprite);
			quad_lslider->update();
		}
		{
			*event.z += 1.0f;
			quad_rslider->setPosition({ render_position.x + slider_pos.x, render_position.y });
			quad_rslider->setZ(*event.z);
			quad_rslider->setSize({ size.x - slider_pos.x, size.y });
			quad_rslider->setColor(slider_info.slider_rcolor);
			quad_rslider->setSprite(slider_info.slider_sprite);
			quad_rslider->update();
		}

		*event.z += 1.0f;
		quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos) + glm::ivec2(slider_info.knob_size.x / 2, slider_info.slider_size.y / 2)));
		quad_knob->setZ(*event.z);
		quad_knob->setSize(slider_info.knob_size);
		quad_knob->setColor(slider_info.knob_color);
		quad_knob->setSprite(slider_info.knob_sprite);
		quad_knob->setRotationAlignment(oe::alignments::center_center);
		quad_knob->update();

		if (slider_info.draw_value) {
			*event.z += 1.0f;
			glm::vec2 text_size = glm::vec2(size.y * 0.6f);
			value_label->generate(fmt::format(L"{:.2f}", slider_info.initial_value), m_gui_manager->getWindow(), { 0.0f, 0.0f, 0.0f, 0.2f });
			label_quad->setPosition(static_cast<glm::vec2>(render_position + size / 2));
			label_quad->setZ(*event.z);
			label_quad->setSize(text_size * value_label->getSize());
			label_quad->setSprite(value_label->getSprite());
			label_quad->setColor(oe::colors::white);
			label_quad->setRotationAlignment(oe::alignments::center_center);
			label_quad->update();
		}
	}

	bool check_inside(const glm::vec2& point, const glm::vec2& top_left, const glm::vec2& size)
	{
		return (point.x >= top_left.x &&
				point.x < top_left.x + size.x &&
				point.y >= top_left.y &&
				point.y < top_left.y + size.y);
	}
	
	void Slider::clamp()
	{
		slider_info.initial_value = oe::utils::clamp(slider_info.initial_value, slider_info.min_value, slider_info.max_value);
	}

	void Slider::on_cursor(const CursorPosEvent& event)
	{
		if (m_dragging) {
			slider_info.initial_value =	oe::utils::map(
				static_cast<float>(event.cursor_windowspace.x - render_position.x - (slider_info.knob_size.x / 2)), 
				0.0f, 
				static_cast<float>(size.x - slider_info.knob_size.x), 
				slider_info.min_value, 
				slider_info.max_value
			);
			clamp();

			if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		}
	}

	void Slider::on_button(const MouseButtonEvent& event)
	{
		if (check_inside(event.cursor_pos.cursor_windowspace, render_position, size))
		{
			// hold
			if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::press)
				m_dragging = true;
		}

		// release
		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::release)
			m_dragging = false;
	}

	void Slider::on_scroll(const ScrollEvent& event)
	{
		if (!slider_info.scroll) return;

		const glm::vec2& cursor_window = m_gui_manager->getWindow()->getCursorWindow();
		if (check_inside(cursor_window, render_position, size))
		{
			const float speed = (slider_info.max_value - slider_info.min_value) * 0.03f;
			slider_info.initial_value += speed * event.scroll_delta.y;
			clamp();
			
			if (slider_info.callback) slider_info.callback(slider_info.initial_value);
		}
	}

}