#include "slider.hpp"

#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"



namespace oe::gui
{
	class SliderLinearRenderer
	{
	private:
		static SliderLinearRenderer* singleton;
		SliderLinearRenderer(const SliderLinearRenderer& copy) = delete;
		SliderLinearRenderer()
			: s_shader()
			, s_renderer(oe::RendererInfo{ 1 })
		{
		}

	public:
		static SliderLinearRenderer& getSingleton() { if (!singleton) singleton = new SliderLinearRenderer(); return *singleton; }

		oe::assets::DefaultShader s_shader;
		oe::graphics::PrimitiveRenderer s_renderer;
	};
	SliderLinearRenderer* SliderLinearRenderer::singleton = nullptr;

	std::string SliderInfo::default_formatter(const SliderInfo& info)
	{
		return fmt::format("{:.1f}", info.value_initial);
	}

	Slider::Slider(const SliderInfo& _slider_info) 
		: Widget(_slider_info.widget_info)
		, m_dragging(false)
		, slider_info(_slider_info)
	{}

	Slider::~Slider()
	{}

	void Slider::managerAssigned()
	{
		if (slider_info.draw_value)
		{
			label_quad = m_gui_manager->getLateRenderer()->create();
			value_label = new oe::graphics::TextLabel(m_gui_manager->getFont(slider_info.draw_font_size, slider_info.draw_font_path));
		}
		quad_knob = m_gui_manager->getRenderer()->create();
		if (!slider_info.linear_color)
		{
			quad_lslider = m_gui_manager->getRenderer()->create();
			quad_rslider = m_gui_manager->getRenderer()->create();
		}

		// event listeners
		m_gui_manager->getWindow()->connect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		m_gui_manager->getWindow()->connect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		m_gui_manager->getWindow()->connect_listener<oe::ScrollEvent, &Slider::on_scroll>(this);
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Slider::on_render>(this);
	}

	void Slider::managerUnassigned()
	{
		if (slider_info.draw_value)
		{
			label_quad.reset();
			delete value_label;
		}
		quad_knob.reset();
		if (!slider_info.linear_color)
		{
			quad_lslider.reset();
			quad_rslider.reset();
		}

		// event listeners
		m_gui_manager->getWindow()->disconnect_listener<oe::MouseButtonEvent, &Slider::on_button>(this);
		m_gui_manager->getWindow()->disconnect_listener<oe::CursorPosEvent, &Slider::on_cursor>(this);
		m_gui_manager->getWindow()->disconnect_listener<oe::ScrollEvent, &Slider::on_scroll>(this);
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Slider::on_render>(this);
	}
	
	void Slider::on_render(const GUIRenderEvent& event)
	{
		quad_knob->toggle(m_info.toggled);
		if (!slider_info.linear_color)
		{
			quad_lslider->toggle(m_info.toggled);
			quad_rslider->toggle(m_info.toggled);
		}
		if (slider_info.draw_value)
			label_quad->toggle(slider_info.draw_value);
		if (!m_info.toggled)
		{
			quad_knob->update(quad_knob);
			if (!slider_info.linear_color)
			{
				quad_lslider->update(quad_lslider);
				quad_rslider->update(quad_rslider);
			}
			if (slider_info.draw_value)
				label_quad->update(label_quad);
			return;
		}

		if (!slider_info.slider_sprite) slider_info.slider_sprite = slider_info.knob_sprite;
		if (!slider_info.knob_sprite) slider_info.knob_sprite = slider_info.slider_sprite;
		NULL_SPRITE_CHECK(!(!slider_info.slider_sprite || !slider_info.knob_sprite));

		if (!slider_info.linear_color)
		{
			if (slider_info.vertical)
			{
				glm::vec2 slider_pos = glm::vec2(0.0f, oe::utils::map(slider_info.value_initial, slider_info.value_bounds.x, slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.y - slider_info.knob_size.y)));
				{
					quad_lslider->setPosition(render_position);
					quad_lslider->setZ(z);
					quad_lslider->setSize({ m_info.size.x, slider_pos.y });
					quad_lslider->setColor(slider_info.slider_lcolor);
					quad_lslider->setSprite(slider_info.slider_sprite);
					quad_lslider->update(quad_lslider);
				}
				{
					quad_rslider->setPosition({ render_position.x, render_position.y + slider_pos.y });
					quad_rslider->setZ(z + 0.025f);
					quad_rslider->setSize({ m_info.size.x, m_info.size.y - slider_pos.y });
					quad_rslider->setColor(slider_info.slider_rcolor);
					quad_rslider->setSprite(slider_info.slider_sprite);
					quad_rslider->update(quad_rslider);
				}

				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos) + glm::ivec2(m_info.size.x / 2, slider_info.knob_size.y / 2)));
			}
			else
			{
				glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.value_initial, slider_info.value_bounds.x, slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.x - slider_info.knob_size.x)), 0.0f);
				{
					quad_lslider->setPosition(render_position);
					quad_lslider->setZ(z);
					quad_lslider->setSize({ slider_pos.x, m_info.size.y });
					quad_lslider->setColor(slider_info.slider_lcolor);
					quad_lslider->setSprite(slider_info.slider_sprite);
					quad_lslider->update(quad_lslider);
				}
				{
					quad_rslider->setPosition({ render_position.x + slider_pos.x, render_position.y });
					quad_rslider->setZ(z + 0.025f);
					quad_rslider->setSize({ m_info.size.x - slider_pos.x, m_info.size.y });
					quad_rslider->setColor(slider_info.slider_rcolor);
					quad_rslider->setSprite(slider_info.slider_sprite);
					quad_rslider->update(quad_rslider);
				}

				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos) + glm::ivec2(slider_info.knob_size.x / 2, m_info.size.y / 2)));
			}
			
			quad_knob->setZ(z + 0.05f);
			quad_knob->setSize(slider_info.knob_size);
			quad_knob->setColor(slider_info.knob_color);
			quad_knob->setSprite(slider_info.knob_sprite);
			quad_knob->setRotationAlignment(oe::alignments::center_center);
			quad_knob->update(quad_knob);
		}
		else
		{
			auto& renderer = SliderLinearRenderer::getSingleton();
			renderer.s_renderer->clear();
			renderer.s_renderer->begin();
			if (slider_info.vertical)
			{
				std::array<oe::graphics::VertexData, 4> vertices = 
				{{ 
					{ { render_position.x + 0,             render_position.y + 0,             z }, { slider_info.slider_sprite->position.x + 0,                                 slider_info.slider_sprite->position.y + 0 },                                 slider_info.slider_lcolor },
					{ { render_position.x + 0,             render_position.y + m_info.size.y, z }, { slider_info.slider_sprite->position.x + 0,                                 slider_info.slider_sprite->position.y + slider_info.slider_sprite->size.y }, slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + m_info.size.y, z }, { slider_info.slider_sprite->position.x + slider_info.slider_sprite->size.x, slider_info.slider_sprite->position.y + slider_info.slider_sprite->size.y }, slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + 0,             z }, { slider_info.slider_sprite->position.x + slider_info.slider_sprite->size.x, slider_info.slider_sprite->position.y + 0 },                                 slider_info.slider_lcolor },
				}};

				renderer.s_renderer->submitVertex(vertices);

				glm::vec2 slider_pos = glm::vec2(0.0f, oe::utils::map(slider_info.value_initial, slider_info.value_bounds.x, slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.y - slider_info.knob_size.y)));
				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos) + glm::ivec2(m_info.size.x / 2, slider_info.knob_size.y / 2)));
			}
			else
			{
				std::array<oe::graphics::VertexData, 4> vertices =
				{{
					{ { render_position.x + 0,             render_position.y + 0,             z }, { slider_info.slider_sprite->position.x + 0,                                 slider_info.slider_sprite->position.y + 0 },                                 slider_info.slider_lcolor },
					{ { render_position.x + 0,             render_position.y + m_info.size.y, z }, { slider_info.slider_sprite->position.x + 0,                                 slider_info.slider_sprite->position.y + slider_info.slider_sprite->size.y }, slider_info.slider_lcolor },
					{ { render_position.x + m_info.size.x, render_position.y + m_info.size.y, z }, { slider_info.slider_sprite->position.x + slider_info.slider_sprite->size.x, slider_info.slider_sprite->position.y + slider_info.slider_sprite->size.y }, slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + 0,             z }, { slider_info.slider_sprite->position.x + slider_info.slider_sprite->size.x, slider_info.slider_sprite->position.y + 0 },                                 slider_info.slider_rcolor },
				}};

				renderer.s_renderer->submitVertex(vertices);

				glm::vec2 slider_pos = glm::vec2(oe::utils::map(slider_info.value_initial, slider_info.value_bounds.x, slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.x - slider_info.knob_size.x)), 0.0f);
				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos) + glm::ivec2(slider_info.knob_size.x / 2, m_info.size.y / 2)));
			}

			quad_knob->setZ(z + 0.05f);
			quad_knob->setSize(slider_info.knob_size);
			quad_knob->setColor(slider_info.knob_color);
			quad_knob->setSprite(slider_info.knob_sprite);
			quad_knob->setRotationAlignment(oe::alignments::center_center);
			quad_knob->update(quad_knob);

			slider_info.slider_sprite->m_owner->bind();
			m_gui_manager->getShaderFill()->bind();

			renderer.s_renderer->end();
			renderer.s_renderer->render();
		}

		// value
		if (slider_info.draw_value)
		{
			std::string s = slider_info.draw_format(slider_info);
			value_label->generate(s, m_gui_manager->getWindow(), { 0.0f, 0.0f, 0.0f, 0.2f });
			label_quad->setPosition(static_cast<glm::vec2>(render_position + m_info.size / 2));
			label_quad->setZ(z + 0.075f);
			label_quad->setSize(value_label->getSize());
			label_quad->setSprite(value_label->getSprite());
			label_quad->setColor(oe::colors::white);
			label_quad->setRotationAlignment(oe::alignments::center_center);
			label_quad->update(label_quad);
		}
	}
	
	void Slider::clamp()
	{
		slider_info.value_initial = oe::utils::clamp(slider_info.value_initial, slider_info.value_bounds.x, slider_info.value_bounds.y);
	}

	void Slider::on_cursor(const CursorPosEvent& event)
	{
		if (oe::utils::bounding_box_test(event.cursor_windowspace, render_position, m_info.size))
		{
			dispatcher.trigger(event_hover_latest);
		}

		if (m_dragging) {
			if (slider_info.vertical)
				slider_info.value_initial =	oe::utils::map(
					static_cast<float>(event.cursor_windowspace.y - render_position.y - (slider_info.knob_size.y / 2)), 
					0.0f, 
					static_cast<float>(m_info.size.y - slider_info.knob_size.y), 
					slider_info.value_bounds.x, 
					slider_info.value_bounds.y
				);
			else
			{
				slider_info.value_initial =	oe::utils::map(
					static_cast<float>(event.cursor_windowspace.x - render_position.x - (slider_info.knob_size.x / 2)), 
					0.0f, 
					static_cast<float>(m_info.size.x - slider_info.knob_size.x), 
					slider_info.value_bounds.x, 
					slider_info.value_bounds.y
				);
			}
			clamp();
			
			event_use_latest.value = slider_info.value_initial;
			dispatcher.trigger(event_use_latest);
		}
	}

	void Slider::on_button(const MouseButtonEvent& event)
	{
		if (oe::utils::bounding_box_test(event.cursor_pos.cursor_windowspace, render_position, m_info.size))
		{
			// hold
			if (event.button == oe::mouse_buttons::button_left && event.action != oe::actions::release) // press or repeat
			{
				m_dragging = true;

				event_use_latest.action = event.action;
				event_use_latest.button = event.button;
				event_use_latest.modifier = event.mods;
				event_use_latest.value = slider_info.value_initial;
				dispatcher.trigger(event_use_latest);

				on_cursor(event.cursor_pos);
			}
		}

		// release
		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::release)
		{
			m_dragging = false;

			event_use_latest.action = event.action;
			event_use_latest.button = event.button;
			event_use_latest.modifier = event.mods;
			event_use_latest.value = slider_info.value_initial;
			dispatcher.trigger(event_use_latest);
		}
	}

	void Slider::on_scroll(const ScrollEvent& event)
	{
		if (!slider_info.scroll) return;

		const glm::vec2& cursor_window = m_gui_manager->getWindow()->getCursorWindow();
		if (oe::utils::bounding_box_test(cursor_window, render_position, m_info.size))
		{
			const float speed = (slider_info.value_bounds.y - slider_info.value_bounds.x) * 0.03f;
			slider_info.value_initial += speed * event.scroll_delta.y;
			clamp();

			event_use_latest.action = oe::actions::none;
			event_use_latest.button = oe::mouse_buttons::none;
			event_use_latest.modifier = oe::modifiers::none;
			event_use_latest.value = slider_info.value_initial;
			dispatcher.trigger(event_use_latest);
		}
	}

}