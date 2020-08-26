#include "graph.hpp"

#include "engine/gui/gui_manager.hpp"
#include "engine/assets/default_shader/shader.vert.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/random.hpp"



constexpr char graph_shader_frag[] = R"glsl(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

#define LINE_W (3)

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

uniform ivec2 u_viewport;
uniform float u_graph_data[256];
uniform int u_graph_size; /* should always be in range 0-255 */
uniform float u_line_w = 1.0;

float d_line(in vec2 pixel, in vec2 line_a, in vec2 line_b)
{
	float a = distance(line_a, pixel);
  	float b = distance(line_b, pixel);
  	float c = distance(line_a, line_b);

  	if ( a >= c || b >=  c ) return 0.0;

  	float p = (a + b + c) * 0.5;
  	float h = 2.0 / c * sqrt(p * (p - a) * (p - b) * (p - c));
  	return mix(1.0, 0.0, smoothstep(0.5 * u_line_w, 1.5 * u_line_w, h));
}

void main()
{
	// point in screen
	ivec2 pixel = ivec2(gl_FragCoord.xy);
	vec2 pixelf = vec2(pixel) / u_viewport.xy;

	// points of the line
	int left_i = int(floor(pixelf.x * u_graph_size));
	int right_i = left_i + 1;
	ivec2 left = ivec2(left_i * u_viewport.x / u_graph_size, u_graph_data[left_i] * u_viewport.y);
	ivec2 right = ivec2(right_i * u_viewport.x / u_graph_size, u_graph_data[right_i] * u_viewport.y);

	// distance to the line
	float d = d_line(vec2(pixel), vec2(left), vec2(right));

	// if bellow the line
	float t_left_right = mod(pixelf.x * u_graph_size, 1.0);
	int h = int(mix(u_graph_data[left_i] * u_viewport.y, u_graph_data[right_i] * u_viewport.y, t_left_right));

	// final color alpha
	float alpha = max(d, (h >= pixel.y ? 0.5 : 0.0));

	color = vec4(1.0, 1.0, 1.0, alpha);
}
)glsl";



namespace oe::gui
{
	class GraphRenderer
	{
	private:
		static GraphRenderer* singleton;
		GraphRenderer(const GraphRenderer& copy) = delete;
		GraphRenderer()
			: g_shader({ "asset:graph_shader", { { oe::shader_stages::vertex_shader, shader_vert_gl }, { oe::shader_stages::fragment_shader, graph_shader_frag } } })
			, g_renderer(oe::RendererInfo{ 1 })
			, g_pack()
		{
			g_pack.construct();

			auto quad = g_renderer.create();
			quad->setPosition({ -1.0f, -1.0f });
			quad->setSize({ 2.0f, 2.0f });
			quad->setSprite(g_pack.emptySprite());
			quad->update();

			g_renderer.forget(std::move(quad));
		}

	public:
		static GraphRenderer& getSingleton() { if(!singleton) singleton = new GraphRenderer(); return *singleton; }

		oe::graphics::Shader g_shader;
		oe::graphics::Renderer g_renderer;
		oe::graphics::SpritePack g_pack;
	};
	GraphRenderer* GraphRenderer::singleton = nullptr;



    Graph::Graph(const GraphInfo& info)
        : SpritePanel(info.bg_panel_info)
        , m_graph_info(info)
		, m_graph({ info.graph_color, info.bg_panel_info.sprite, info.bg_panel_info.widget_info })
    {
        std::swap(m_graph_info.graph_color, sprite_panel_info.color);
    }

	void Graph::managerAssigned(GUI* gui_manager)
	{
		graph_fb = oe::graphics::FrameBuffer({ m_info.size }, gui_manager->getWindow());

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Graph::on_render>(this);

		SpritePanel::managerAssigned(gui_manager);
	}

	void Graph::managerUnassigned(GUI* gui_manager)
	{
		graph_fb.reset();

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Graph::on_render>(this);

		SpritePanel::managerUnassigned(gui_manager);
	}
	
	void Graph::on_render(const GUIRenderEvent& event)
	{
		graph_fb->bind();
		graph_fb->clear(m_graph_info.graph_color);

		glm::ivec2 viewport = m_info.size;
		if(m_graph_info.graph_data.size() != 0) viewport += glm::ivec2(m_info.size.x / m_graph_info.graph_data.size(), 0.0f);
		auto& graph_renderer = GraphRenderer::getSingleton();
		graph_renderer.g_shader->bind();
		graph_renderer.g_shader->setUniform("u_graph_size", (int)m_graph_info.graph_data.size());
		graph_renderer.g_shader->setUniform("u_graph_data", m_graph_info.graph_data);
		graph_renderer.g_shader->setUniform("u_viewport", viewport);
		graph_renderer.g_shader->setUniform("u_line_w", m_graph_info.graph_line_width);
		graph_renderer.g_renderer.render();

		m_gui_manager->getWindow()->bind();

		sprite_panel_info.sprite = &graph_fb->getSprite();
	}
}