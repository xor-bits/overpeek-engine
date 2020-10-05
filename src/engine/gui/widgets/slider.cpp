#include "slider.hpp"

#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/utility/connect_guard_additions.hpp"



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

	std::u32string SliderInfo::default_formatter(const float& val)
	{
		return fmt::format(U"{:.1f}", val);
	}

	Slider::Slider(Widget* parent, GUI& gui_manager, float& value_ref, const SliderInfo& slider_info) 
		: Widget(parent, gui_manager, slider_info.widget_info)
		, m_dragging(false)
		, m_slider_info(slider_info)
		, m_value(value_ref)
	{
		if (!m_slider_info.slider_sprite) m_slider_info.slider_sprite = m_slider_info.knob_sprite;
		if (!m_slider_info.knob_sprite)   m_slider_info.knob_sprite   = m_slider_info.slider_sprite;
	}
	
	void Slider::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			if (m_slider_info.draw_value)
			{
				label_quad = m_gui_manager.getRenderer()->create();
				value_label = new oe::graphics::u32TextLabel(m_gui_manager.getFont(m_slider_info.font_size, m_slider_info.font_file));
			}
			quad_knob = m_gui_manager.getRenderer()->create();
			if (!m_slider_info.linear_color)
			{
				quad_lslider = m_gui_manager.getRenderer()->create();
				quad_rslider = m_gui_manager.getRenderer()->create();
			}

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &Slider::on_render, Slider>(m_gui_manager.dispatcher, this);
			m_cg_cursor.connect<CursorPosEvent, &Slider::on_cursor, Slider>(m_gui_manager.dispatcher, this);
			m_cg_button.connect<MouseButtonEvent, &Slider::on_button, Slider>(m_gui_manager.dispatcher, this);
			m_cg_scroll.connect<ScrollEvent, &Slider::on_scroll, Slider>(m_gui_manager.dispatcher, this);
		}
		else
		{
			if (m_slider_info.draw_value)
			{
				label_quad.reset();
				delete value_label;
			}
			quad_knob.reset();
			if (!m_slider_info.linear_color)
			{
				quad_lslider.reset();
				quad_rslider.reset();
			}

			// event listeners
			m_cg_render.disconnect();
			m_cg_cursor.disconnect();
			m_cg_button.disconnect();
			m_cg_scroll.disconnect();
		}
	}
	
	void Slider::on_render(const GUIRenderEvent& event)
	{
		if(!m_cg_render)
			return;

		if (!m_slider_info.linear_color)
		{
			if (m_slider_info.vertical)
			{
				glm::ivec2 slider_pos = glm::vec2(0.0f, oe::utils::map(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.y - m_slider_info.knob_size.y)));
				
				{
					quad_lslider->setPosition(render_position);
					quad_lslider->setZ(z);
					quad_lslider->setSize({ m_info.size.x, slider_pos.y });
					quad_lslider->setColor(m_slider_info.slider_lcolor);
					quad_lslider->setSprite(m_slider_info.slider_sprite);
				}
				{
					quad_rslider->setPosition({ render_position.x, render_position.y + slider_pos.y });
					quad_rslider->setZ(z + 0.025f);
					quad_rslider->setSize({ m_info.size.x, m_info.size.y - slider_pos.y });
					quad_rslider->setColor(m_slider_info.slider_rcolor);
					quad_rslider->setSprite(m_slider_info.slider_sprite);
				}

				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos)));
			}
			else
			{
				glm::ivec2 slider_pos = glm::vec2(oe::utils::map(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.x - m_slider_info.knob_size.x)), 0.0f);

				{
					quad_lslider->setPosition(render_position);
					quad_lslider->setZ(z);
					quad_lslider->setSize({ slider_pos.x, m_info.size.y });
					quad_lslider->setColor(m_slider_info.slider_lcolor);
					quad_lslider->setSprite(m_slider_info.slider_sprite);
				}
				{
					quad_rslider->setPosition({ render_position.x + slider_pos.x, render_position.y });
					quad_rslider->setZ(z + 0.025f);
					quad_rslider->setSize({ m_info.size.x - slider_pos.x, m_info.size.y });
					quad_rslider->setColor(m_slider_info.slider_rcolor);
					quad_rslider->setSprite(m_slider_info.slider_sprite);
				}

				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos)));
			}
			
			quad_knob->setZ(z + 0.05f);
			quad_knob->setSize(m_slider_info.knob_size);
			quad_knob->setColor(m_slider_info.knob_color);
			quad_knob->setSprite(m_slider_info.knob_sprite);
		}
		else
		{
			auto& renderer = SliderLinearRenderer::getSingleton();
			renderer.s_renderer->clear();
			renderer.s_renderer->begin();
			if (m_slider_info.vertical)
			{
				std::array<oe::graphics::VertexData, 4> vertices = 
				{{ 
					{ { render_position.x + 0,             render_position.y + 0,             z }, { m_slider_info.slider_sprite->position.x + 0,                                 m_slider_info.slider_sprite->position.y + 0 },                                 m_slider_info.slider_lcolor },
					{ { render_position.x + 0,             render_position.y + m_info.size.y, z }, { m_slider_info.slider_sprite->position.x + 0,                                 m_slider_info.slider_sprite->position.y + m_slider_info.slider_sprite->size.y }, m_slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + m_info.size.y, z }, { m_slider_info.slider_sprite->position.x + m_slider_info.slider_sprite->size.x, m_slider_info.slider_sprite->position.y + m_slider_info.slider_sprite->size.y }, m_slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + 0,             z }, { m_slider_info.slider_sprite->position.x + m_slider_info.slider_sprite->size.x, m_slider_info.slider_sprite->position.y + 0 },                                 m_slider_info.slider_lcolor },
				}};

				renderer.s_renderer->submitVertex(vertices);

				glm::ivec2 slider_pos = glm::vec2(0.0f, oe::utils::map(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.y - m_slider_info.knob_size.y)));
				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos)));
			}
			else
			{
				std::array<oe::graphics::VertexData, 4> vertices =
				{{
					{ { render_position.x + 0,             render_position.y + 0,             z }, { m_slider_info.slider_sprite->position.x + 0,                                 m_slider_info.slider_sprite->position.y + 0 },                                 m_slider_info.slider_lcolor },
					{ { render_position.x + 0,             render_position.y + m_info.size.y, z }, { m_slider_info.slider_sprite->position.x + 0,                                 m_slider_info.slider_sprite->position.y + m_slider_info.slider_sprite->size.y }, m_slider_info.slider_lcolor },
					{ { render_position.x + m_info.size.x, render_position.y + m_info.size.y, z }, { m_slider_info.slider_sprite->position.x + m_slider_info.slider_sprite->size.x, m_slider_info.slider_sprite->position.y + m_slider_info.slider_sprite->size.y }, m_slider_info.slider_rcolor },
					{ { render_position.x + m_info.size.x, render_position.y + 0,             z }, { m_slider_info.slider_sprite->position.x + m_slider_info.slider_sprite->size.x, m_slider_info.slider_sprite->position.y + 0 },                                 m_slider_info.slider_rcolor },
				}};

				renderer.s_renderer->submitVertex(vertices);

				glm::ivec2 slider_pos = glm::vec2(oe::utils::map(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y, 0.0f, static_cast<float>(m_info.size.x - m_slider_info.knob_size.x)), 0.0f);
				quad_knob->setPosition(static_cast<glm::vec2>(render_position + static_cast<glm::ivec2>(slider_pos)));
			}

			quad_knob->setZ(z + 0.05f);
			quad_knob->setSize(m_slider_info.knob_size);
			quad_knob->setColor(m_slider_info.knob_color);
			quad_knob->setSprite(m_slider_info.knob_sprite);

			m_slider_info.slider_sprite->m_owner->bind();
			m_gui_manager.getShaderFill()->bind();

			renderer.s_renderer->end();
			renderer.s_renderer->render();
		}

		// value
		if (m_slider_info.draw_value)
		{
			std::u32string s = m_slider_info.draw_format(m_value);
			value_label->generate(s, m_gui_manager.getWindow(), { 0.0f, 0.0f, 0.0f, 0.2f });
			glm::ivec2 size = value_label->getSize();
			glm::ivec2 position =
				+ render_position
				+ oe::alignmentOffset(m_info.size, oe::alignments::center_center)
				- oe::alignmentOffset(size, oe::alignments::center_center);

			label_quad->setPosition(static_cast<glm::vec2>(position));
			label_quad->setZ(z + 0.075f);
			label_quad->setSize(static_cast<glm::vec2>(size));
			label_quad->setSprite(value_label->getSprite());
			label_quad->setColor(oe::colors::white);
		}
	}
	
	void Slider::clamp()
	{
		m_value = oe::utils::clamp(m_value, m_slider_info.value_bounds.x, m_slider_info.value_bounds.y);
		const float range = std::fabs(m_slider_info.value_bounds.x - m_slider_info.value_bounds.y);
		m_value *= range * m_slider_info.value_steps;
		m_value = std::round(m_value);
		m_value /= range * m_slider_info.value_steps;
	}

	void Slider::on_cursor(const CursorPosEvent& event)
	{
		if(!(m_slider_info.interact_flags & interact_type_flags::cursor))
			return;

		if (oe::utils::bounding_box_test(event.cursor_windowspace, render_position, m_info.size))
		{
			dispatcher.trigger(m_event_hover_latest);
		}

		if (m_dragging) {
			if (m_slider_info.vertical)
				m_value =	oe::utils::map(
					static_cast<float>(event.cursor_windowspace.y - render_position.y - (m_slider_info.knob_size.y / 2)), 
					0.0f, 
					static_cast<float>(m_info.size.y - m_slider_info.knob_size.y), 
					m_slider_info.value_bounds.x, 
					m_slider_info.value_bounds.y
				);
			else
			{
				m_value =	oe::utils::map(
					static_cast<float>(event.cursor_windowspace.x - render_position.x - (m_slider_info.knob_size.x / 2)), 
					0.0f, 
					static_cast<float>(m_info.size.x - m_slider_info.knob_size.x), 
					m_slider_info.value_bounds.x, 
					m_slider_info.value_bounds.y
				);
			}
			clamp();
			
			m_event_use_latest.value = m_value;
			dispatcher.trigger(m_event_use_latest);
		}
	}

	void Slider::on_button(const MouseButtonEvent& event)
	{
		if(!(m_slider_info.interact_flags & interact_type_flags::cursor))
			return;

		if (oe::utils::bounding_box_test(event.cursor_pos.cursor_windowspace, render_position, m_info.size))
		{
			// hold
			if (event.button == oe::mouse_buttons::button_left && event.action != oe::actions::release) // press or repeat
			{
				m_dragging = true;

				m_event_use_latest.action = event.action;
				m_event_use_latest.button = event.button;
				m_event_use_latest.modifier = event.mods;
				m_event_use_latest.value = m_value;
				dispatcher.trigger(m_event_use_latest);

				on_cursor(event.cursor_pos);
			}
		}

		// release
		if (event.button == oe::mouse_buttons::button_left && event.action == oe::actions::release)
		{
			m_dragging = false;

			m_event_use_latest.action = event.action;
			m_event_use_latest.button = event.button;
			m_event_use_latest.modifier = event.mods;
			m_event_use_latest.value = m_value;
			dispatcher.trigger(m_event_use_latest);
		}
	}

	void Slider::on_scroll(const ScrollEvent& event)
	{
		if (!(m_slider_info.interact_flags & interact_type_flags::scroll))
			return;

		const glm::vec2& cursor_window = m_gui_manager.getWindow()->getCursorWindow();
		if (oe::utils::bounding_box_test(cursor_window, render_position, m_info.size))
		{
			const float speed = (m_slider_info.value_bounds.y - m_slider_info.value_bounds.x) * 0.03f;
			m_value += speed * event.scroll_delta.y;
			clamp();

			m_event_use_latest.action = oe::actions::none;
			m_event_use_latest.button = oe::mouse_buttons::none;
			m_event_use_latest.modifier = oe::modifiers::none;
			m_event_use_latest.value = m_value;
			dispatcher.trigger(m_event_use_latest);
		}
	}

}