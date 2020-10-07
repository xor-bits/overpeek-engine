#include "color_picker.hpp"

#include "engine/interfacegen_renderer.hpp"

#include "engine/gui/gui_manager.hpp"

#include "engine/graphics/interface/index_buffer_gen.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/connect_guard_additions.hpp"

#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/assets/default_shader/shader.vert.hpp"
#include "engine/assets/texture_set/texture_set.hpp"



constexpr char color_picker_shader_frag[] = R"glsl(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform ivec2 u_offset;
uniform ivec2 u_viewport;
uniform int u_usetex = 1;
uniform float u_wheel_width = 0.1;
uniform bool u_hsv = true;


vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float aspect = float(u_viewport.x) / float(u_viewport.y);
	float wheel_width = u_wheel_width * 0.5;
    
	vec2 point = (gl_FragCoord.xy + u_offset) / vec2(u_viewport.xy);
    point.x *= aspect;

	vec4 hsv = vec4(1.0);
	if (u_hsv)
	{
		float dir = atan(point.y, point.x);
		hsv = vec4(hsv2rgb(vec3(dir / 6.28, 1.0, 1.0)), 1.0);
	}

	color = vec4(0.0);
	float distance_from_middle = length(point);
    distance_from_middle = 1.0 - abs(0.49 - wheel_width - distance_from_middle) + wheel_width;
	
	if(distance_from_middle > 1.0f)
		color = hsv * distance_from_middle;
}
)glsl";

namespace oe::gui
{
	bool in_circle(const glm::vec2& p, float width)
	{
		float l = glm::length(p);
		return l < 1.0f && l > 1.0f - width * 2;
	}

	float sign(const std::array<glm::vec2, 3>& points)
	{
		return (points[0].x - points[2].x) * (points[1].y - points[2].y) - (points[1].x - points[2].x) * (points[0].y - points[2].y);
	}

	bool in_triangle(const glm::vec2& p, const std::array<glm::vec2, 3>& triangle)
	{
		float d1, d2, d3;
		bool has_neg, has_pos;

		d1 = sign({ p, triangle[0], triangle[1] });
		d2 = sign({ p, triangle[1], triangle[2] });
		d3 = sign({ p, triangle[2], triangle[0] });

		has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		return !(has_neg && has_pos);
	}

	glm::vec2 relative_pos(const glm::ivec2& pos_windowspace_a, const glm::ivec2& pos_windowspace_b, const glm::ivec2& size)
	{
		return static_cast<glm::vec2>((pos_windowspace_a - pos_windowspace_b) - size / 2) / static_cast<glm::vec2>(size / 2);
	}

	template<size_t dim = 2>
	glm::vec3 to_barycentric(const glm::vec<dim, float>& p, const std::array<glm::vec<dim, float>, 3>& triangle)
	{
		glm::vec<dim, float> v0 = triangle[1] - triangle[0], v1 = triangle[2] - triangle[0], v2 = p - triangle[0];
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;

		glm::vec3 barycentric;
		barycentric.z = (d00 * d21 - d01 * d20) / denom;
		barycentric.y = (d11 * d20 - d01 * d21) / denom;
		barycentric.x = 1.0f - barycentric.z - barycentric.y;
		return barycentric;
	}

	glm::vec3 clamp_barycentric(const glm::vec3& barycentric_pos, const std::array<glm::vec2, 3>& triangle)
	{
		glm::vec2 p = barycentric_pos.x * triangle[0] + barycentric_pos.y * triangle[1] + barycentric_pos.z * triangle[2];
		if (barycentric_pos.x < 0)
		{
			float t = glm::dot(p - triangle[1], triangle[2] - triangle[1]) / glm::dot(triangle[2] - triangle[1], triangle[2] - triangle[1]);
			t = glm::clamp(t, 0.0f, 1.0f);
			return glm::vec3(0.0f, 1.0f - t, t);
		}
		else if (barycentric_pos.y < 0)
		{
			float t = glm::dot(p - triangle[2], triangle[0] - triangle[2]) / glm::dot(triangle[0] - triangle[2], triangle[0] - triangle[2]);
			t = glm::clamp(t, 0.0f, 1.0f);
			return glm::vec3(t, 0.0f, 1.0f - t);
		}
		else if (barycentric_pos.z < 0)
		{
			float t = glm::dot(p - triangle[0], triangle[1] - triangle[0]) / glm::dot(triangle[1] - triangle[0], triangle[1] - triangle[0]);
			t = glm::clamp(t, 0.0f, 1.0f);
			return glm::vec3(1.0f - t, t, 0.0f);
		}
		else
		{
			return barycentric_pos;
		}
	}

	class ColorPickerRenderer
	{
	private:
		static ColorPickerRenderer* singleton;
		ColorPickerRenderer(const ColorPickerRenderer& copy) = delete;
		ColorPickerRenderer()
			: c_shader({ "asset:graph_shader", { { oe::shader_stages::vertex_shader, shader_vert_gl }, { oe::shader_stages::fragment_shader, color_picker_shader_frag } } })
			, c_renderer_circle(oe::RendererInfo{ 1 })
			, c_renderer_triangle(oe::RendererInfo{ 1 })
			, c_pack()
		{
			auto checker_img = oe::assets::TextureSet::generate_checkerboard();
			TextureInfo ti;
			ti.data = checker_img.data;
			ti.data_format = checker_img.format;
			ti.size_offset = { { checker_img.width, 0 }, { checker_img.height, 0 } };
			ti.wrap = oe::texture_wrap::repeat;

			c_checkerboard = {
				{ ti }, { 0.0f, 0.0f }, { 1.0f, 1.0f }
			};

			auto circle_img = oe::assets::TextureSet::generate_circle(32);
			c_circle_sprite = c_pack.create(circle_img);
			c_pack.construct();

			c_renderer_circle->begin();
			c_renderer_circle->submitVertex(std::array<oe::graphics::VertexData, 4> {{
				{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }, oe::colors::white },
				{ { -1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, oe::colors::white },
				{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, oe::colors::white },
				{ { 1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }, oe::colors::white },
			}}, 0);
			c_renderer_circle->end();
		}

	public:
		static ColorPickerRenderer& getSingleton() { if (!singleton) singleton = new ColorPickerRenderer(); return *singleton; }

		oe::graphics::Shader c_shader;
		oe::assets::DefaultShader c_defaultshader;
		oe::graphics::PrimitiveRenderer c_renderer_circle;
		oe::graphics::TrianglePrimitiveRenderer c_renderer_triangle;
		oe::graphics::Sprite const* c_circle_sprite;
		oe::graphics::SpritePack c_pack;
		oe::graphics::Sprite c_checkerboard;
	};
	ColorPickerRenderer* ColorPickerRenderer::singleton = nullptr;



	ColorPicker::ColorPicker(Widget* parent, GUI& gui_manager, value_t& value_ref, const ColorPickerInfo& color_picker_info)
		: SpritePanel(parent, gui_manager, { color_picker_info.color_input_info.background_color, color_picker_info.color_input_info.sprite, 0.0f, color_picker_info.color_input_info.widget_info })
		, m_color_picker_info(color_picker_info)
		, m_value(value_ref)
	{
		m_triangle_vertices[0].color = oe::colors::red;
		m_triangle_vertices[1].color = oe::colors::white;
		m_triangle_vertices[2].color = oe::colors::black;

		m_triangle_vertices[0].uv = { 0.0f, 0.0f };
		m_triangle_vertices[1].uv = { 0.0f, 0.0f };
		m_triangle_vertices[2].uv = { 0.0f, 0.0f };

		auto& renderer = ColorPickerRenderer::getSingleton();

		SpritePanelInfo lsp_info;
		lsp_info.widget_info.size = glm::ivec2{ std::min(m_info.size.x, m_info.size.y - 25) };
		lsp_info.widget_info.align_parent = oe::alignments::top_center;
		lsp_info.widget_info.align_render = oe::alignments::top_center;
		lsp_info.widget_info.toggled = color_picker_info.color_input_info.widget_info.toggled;
		lsp_info.sprite = &renderer.c_checkerboard;
		lsp_info.color = oe::colors::white;
		m_framebuffer_panel = create<SpritePanel>(lsp_info);

		if (color_picker_info.preview)
		{
			SpritePanelInfo sp_info;
			sp_info.widget_info.size = { 25, 25 };
			sp_info.widget_info.offset_position = { 5, 5 };
			sp_info.widget_info.align_parent = oe::alignments::top_left;
			sp_info.widget_info.align_render = oe::alignments::top_left;
			sp_info.widget_info.toggled = color_picker_info.color_input_info.widget_info.toggled;
			sp_info.sprite = renderer.c_circle_sprite;
			sp_info.color = oe::colors::white;
			m_preview = create<SpritePanel>(sp_info);
		}

		if (color_picker_info.alpha)
		{
			SliderInputInfo s_info;
			s_info.knob_size = { 16, 16 };
			s_info.widget_info.size = { color_picker_info.color_input_info.widget_info.size.x - 10, 15 };
			s_info.widget_info.offset_position = { 0, -5 };
			s_info.widget_info.align_parent = oe::alignments::bottom_center;
			s_info.widget_info.align_render = oe::alignments::bottom_center;
			s_info.widget_info.toggled = color_picker_info.color_input_info.widget_info.toggled;
			s_info.slider_sprite = &renderer.c_checkerboard;
			s_info.knob_sprite = &renderer.c_checkerboard;
			s_info.value_bounds = { 0.0f, 1.0f };
			s_info.linear_color = true;
			s_info.slider_lcolor = oe::colors::white;
			s_info.slider_rcolor = oe::colors::black;
			s_info.knob_sprite = color_picker_info.color_input_info.sprite;
			m_alpha_slider = create<SliderInput>(m_value.a, s_info);
		}

		m_value_last = m_value + 1.0f;
		sprite_panel_info.sprite = m_color_picker_info.color_input_info.sprite;

		m_alpha_slider->connect_listener<SliderInputUseEvent, &ColorPicker::on_slider_use>(this);
	}

	ColorPicker::~ColorPicker()
	{
		m_alpha_slider->disconnect_listener<SliderInputUseEvent, &ColorPicker::on_slider_use>(this);
	}

	void ColorPicker::on_slider_use(const SliderInputUseEvent& event)
	{
		m_value.a = event.value;
	}
	
	void ColorPicker::virtual_toggle(bool enabled)
	{
		SpritePanel::virtual_toggle(enabled);
		if(enabled)
		{
			m_wheel_fb = { { m_framebuffer_panel->m_info.size } };
			m_framebuffer_panel->sprite_panel_info.sprite = &m_wheel_fb->getSprite();

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &ColorPicker::on_render, ColorPicker>(m_gui_manager.dispatcher, this);
			m_cg_cursor.connect<CursorPosEvent, &ColorPicker::on_cursor, ColorPicker>(m_gui_manager.dispatcher, this);
			m_cg_button.connect<MouseButtonEvent, &ColorPicker::on_button, ColorPicker>(m_gui_manager.dispatcher, this);
		}
		else
		{
			m_wheel_fb.reset();

			// event listeners
			m_cg_render.disconnect();
			m_cg_cursor.disconnect();
			m_cg_button.disconnect();
		}
	}

	/* const glm::vec4& ColorPicker::get() const
	{
		return m_value;
	}

	void ColorPicker::set(const glm::vec4& color)
	{
		if (m_value == color)
			return;
		m_value = color;
		m_alpha_slider->slider_info.value_initial = color.a;

		glm::vec3 hsv = oe::utils::rgbToHSV(color);
		triangle_vertices[0].color = { oe::utils::hsvToRGB({ hsv.x, 1.0f, 1.0f }), 1.0f };
		direction = -hsv.x * glm::two_pi<float>();

		glm::vec3 left = hsv.z * (hsv.y) * glm::vec3{ 1.0f, 0.0f, 0.0f };
		glm::vec3 right = hsv.z * (1.0f - hsv.y) * glm::vec3{ 0.0f, 1.0f, 0.0f };
		glm::vec3 origin = (1.0f - hsv.z * (hsv.y) - hsv.z * (1.0f - hsv.y)) * glm::vec3{ 0.0f, 0.0f, 1.0f };

		origin = origin + left + right;
		barycentric_pos_triangle = origin;

		if (m_color_picker_info.preview)
		{
			m_preview->sprite_panel_info.color = m_value;
		}

		update();
	} */
	
	void ColorPicker::update_from_value(bool update_dir)
	{
		m_value_last = m_value;

		// set color
		if(update_dir)
		{
			glm::vec3 hsv = oe::utils::rgbToHSV(m_value);
			m_triangle_vertices[0].color = { oe::utils::hsvToRGB({ hsv.x, 1.0f, 1.0f }), 1.0f };
			m_direction = -hsv.x * glm::two_pi<float>();

			glm::vec3 left = hsv.z * (hsv.y) * glm::vec3{ 1.0f, 0.0f, 0.0f };
			glm::vec3 right = hsv.z * (1.0f - hsv.y) * glm::vec3{ 0.0f, 1.0f, 0.0f };
			glm::vec3 origin = (1.0f - hsv.z * (hsv.y) - hsv.z * (1.0f - hsv.y)) * glm::vec3{ 0.0f, 0.0f, 1.0f };

			origin = origin + left + right;
			m_barycentric_pos_triangle = origin;
		}
		
		// triangle vertices
		m_triangle_vertices[0].position = { std::cos(m_direction + 0 * m_equilateral_triangle_angles) * m_triangle_width, std::sin(m_direction + 0 * m_equilateral_triangle_angles) * m_triangle_width, 0.0f };
		m_triangle_vertices[1].position = { std::cos(m_direction + 1 * m_equilateral_triangle_angles) * m_triangle_width, std::sin(m_direction + 1 * m_equilateral_triangle_angles) * m_triangle_width, 0.0f };
		m_triangle_vertices[2].position = { std::cos(m_direction + 2 * m_equilateral_triangle_angles) * m_triangle_width, std::sin(m_direction + 2 * m_equilateral_triangle_angles) * m_triangle_width, 0.0f };

		// preview color
		if (m_color_picker_info.preview)
			m_preview->sprite_panel_info.color = m_value;

		// circle in wheel
		glm::vec2 selector_wheel_f = { std::cos(m_direction), std::sin(m_direction) };
		selector_wheel_f *= -m_framebuffer_panel->m_info.size / 2;
		selector_wheel_f.y *= -1.0f;
		m_selector_wheel = glm::vec2(selector_wheel_f) * (1.0f - m_wheel_width);

		// circle in triangle
		glm::vec2 selector_triangle_f =
			+ m_barycentric_pos_triangle.x * m_triangle_vertices[0].position
			+ m_barycentric_pos_triangle.y * m_triangle_vertices[1].position
			+ m_barycentric_pos_triangle.z * m_triangle_vertices[2].position;
		selector_triangle_f *= -m_framebuffer_panel->m_info.size / 2;
		selector_triangle_f.y *= -1.0f;
		m_selector_triangle = selector_triangle_f;
	}
	
	void ColorPicker::update_to_value()
	{
		update_from_value(false);

		// set color according to triangle and wheel
		float alpha = m_alpha_slider ? m_alpha_slider->m_value : m_value.a;
		m_value =
			+ m_barycentric_pos_triangle.x * m_triangle_vertices[0].color
			+ m_barycentric_pos_triangle.y * m_triangle_vertices[1].color
			+ m_barycentric_pos_triangle.z * m_triangle_vertices[2].color;
		m_value = glm::max(m_value, 0.0f);
		m_value.a = alpha;
		m_value_last = m_value;

		// preview color
		if (m_color_picker_info.preview)
			m_preview->sprite_panel_info.color = m_value;

		m_event_use_latest.value = m_value;
		dispatcher.trigger(m_event_use_latest);
	}

	void ColorPicker::on_render(const GUIRenderEvent& event)
	{
		if(!m_cg_render)
			return;

		if(m_value_last != m_value)
			update_from_value(true);

		m_wheel_fb->bind();
		m_wheel_fb->clear(oe::colors::transparent);

		float aspect = static_cast<float>(m_framebuffer_panel->m_info.size.x) / static_cast<float>(m_framebuffer_panel->m_info.size.y);
		glm::mat4 pr_matrix = glm::ortho(-aspect, aspect, 1.0f, -1.0f);

		auto& renderer = ColorPickerRenderer::getSingleton();
		renderer.c_shader->bind();
		renderer.c_shader->setUniform("u_viewport", m_framebuffer_panel->m_info.size);
		renderer.c_shader->setUniform("u_offset", -m_framebuffer_panel->m_info.size / 2);
		renderer.c_shader->setUniform("u_wheel_width", m_wheel_width);
		renderer.c_shader->setUniform("mvp_matrix", pr_matrix);
		renderer.c_shader->setUniform("u_hsv", true);
		renderer.c_renderer_circle->render();

		renderer.c_defaultshader.bind();
		renderer.c_defaultshader.setProjectionMatrix(pr_matrix);
		renderer.c_defaultshader.setColor(oe::colors::white);
		renderer.c_defaultshader.setTexture(false);
		renderer.c_renderer_triangle->begin();
		renderer.c_renderer_triangle->submitVertex(m_triangle_vertices, 0);
		renderer.c_renderer_triangle->end();
		renderer.c_renderer_triangle->render();

		renderer.c_shader->bind();
		renderer.c_shader->setUniform("u_viewport", m_framebuffer_panel->m_info.size / 16);
		renderer.c_shader->setUniform("u_offset", -m_framebuffer_panel->m_info.size / 2 + m_selector_triangle);
		renderer.c_shader->setUniform("u_wheel_width", m_wheel_width);
		renderer.c_shader->setUniform("mvp_matrix", pr_matrix);
		renderer.c_shader->setUniform("u_hsv", false);
		renderer.c_renderer_circle->render();

		renderer.c_shader->bind();
		renderer.c_shader->setUniform("u_viewport", m_framebuffer_panel->m_info.size / 16);
		renderer.c_shader->setUniform("u_offset", -m_framebuffer_panel->m_info.size / 2 + m_selector_wheel);
		renderer.c_shader->setUniform("u_wheel_width", m_wheel_width);
		renderer.c_shader->setUniform("mvp_matrix", pr_matrix);
		renderer.c_shader->setUniform("u_hsv", false);
		renderer.c_renderer_circle->render();

		renderer.c_checkerboard.size = m_alpha_slider->m_info.size / 8;

		m_gui_manager.getWindow()->bind();
		m_framebuffer_panel->sprite_panel_info.sprite = &m_wheel_fb->getSprite();
	}

	void ColorPicker::on_cursor(const CursorPosEvent& event)
	{
		if(!m_cg_cursor)
			return;
		// relative_pos
		glm::vec2 r_pos = relative_pos(event.cursor_windowspace, m_framebuffer_panel->render_position, m_framebuffer_panel->m_info.size);

		if (m_dragging_wheel)
		{
			/*if (!in_circle(r_pos, m_wheel_width))
				return;*/

			m_direction = atan2f(r_pos.y, r_pos.x);
			const float hue = -m_direction / glm::two_pi<float>();
			m_triangle_vertices[0].color = { oe::utils::hsvToRGB({ hue, 1.0f, 1.0f }), 1.0f };

			update_to_value();
		}
		if (m_dragging_triangle)
		{
			/*if (!in_triangle(r_pos, { m_triangle_vertices[0].position, m_triangle_vertices[1].position, m_triangle_vertices[2].position }))
				return;*/

			m_barycentric_pos_triangle = to_barycentric<2>(r_pos, { m_triangle_vertices[0].position, m_triangle_vertices[1].position, m_triangle_vertices[2].position });
			m_barycentric_pos_triangle = clamp_barycentric(m_barycentric_pos_triangle, { m_triangle_vertices[0].position, m_triangle_vertices[1].position, m_triangle_vertices[2].position });
			
			update_to_value();
		}

		if (in_circle(r_pos, m_wheel_width) || in_triangle(r_pos, { m_triangle_vertices[0].position, m_triangle_vertices[1].position, m_triangle_vertices[2].position }))
		{
			dispatcher.trigger(m_event_hover_latest);
		}
	}

	void ColorPicker::on_button(const MouseButtonEvent& event)
	{
		if(!m_cg_button)
			return;

		m_event_use_latest.action = event.action;
		m_event_use_latest.button = event.button;
		m_event_use_latest.modifier = event.mods;

		glm::vec2 r_pos = relative_pos(event.cursor_pos.cursor_windowspace, m_framebuffer_panel->render_position, m_framebuffer_panel->m_info.size);
		if (event.button == oe::mouse_buttons::button_left)
		{
			m_dragging_wheel = event.action != oe::actions::release && in_circle(r_pos, m_wheel_width);
			m_dragging_triangle = event.action != oe::actions::release && in_triangle(r_pos, { m_triangle_vertices[0].position, m_triangle_vertices[1].position, m_triangle_vertices[2].position });
		}

		if (m_dragging_wheel || m_dragging_triangle)
			on_cursor(event.cursor_pos);
	}
}