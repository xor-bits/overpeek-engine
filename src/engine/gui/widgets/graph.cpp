#include "graph.hpp"

#include "engine/gui/gui_manager.hpp"
#include "engine/assets/default_shader/shader.vert.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/random.hpp"
#include "engine/utility/connect_guard_additions.hpp"



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
			: g_shader({ "asset:graph_shader", { { oe::shader_stages::vertex_shader, shader_vert_gl, {} }, { oe::shader_stages::fragment_shader, graph_shader_frag, {} } } })
			, g_renderer(1)
			, g_pack()
		{
			g_pack.construct();

			auto quad = g_renderer.create();
			quad->setPosition({ -1.0f, -1.0f });
			quad->setSize({ 2.0f, 2.0f });
			quad->setSprite(g_pack.emptySprite());

			g_renderer.forget(std::move(quad));
		}

	public:
		static GraphRenderer& getSingleton() { if(!singleton) singleton = new GraphRenderer(); return *singleton; }

		oe::graphics::Shader g_shader;
		oe::graphics::Renderer g_renderer;
		oe::graphics::SpritePack g_pack;
	};
	GraphRenderer* GraphRenderer::singleton = nullptr;



    Graph::Graph(Widget* parent, GUI& gui_manager, const GraphInfo& info)
        : SpritePanel(parent, gui_manager, info.bg_panel_info)
        , m_graph_info(info)
    {
		SpritePanelInfo spi = { oe::colors::transparent, m_graph_info.bg_panel_info.sprite, m_graph_info.bg_panel_info.rotation, m_graph_info.bg_panel_info.widget_info };
		m_graph = create<SpritePanel, SpritePanelInfo>(spi);
		m_graph->m_info.align_parent = oe::alignments::top_left;
		m_graph->m_info.align_render = oe::alignments::top_left;
		m_graph->m_info.offset_position = { 0, 0 };
		m_graph->sprite_panel_info.sprite = nullptr;
    }
	
	void Graph::virtual_toggle(bool enabled)
	{
		SpritePanel::virtual_toggle(enabled);
		if(enabled)
		{
			graph_fb = oe::graphics::FrameBuffer({ m_info.size });

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &Graph::on_render, Graph>(m_gui_manager.dispatcher, this);
		}
		else
		{
			graph_fb.reset();

			// event listeners
			m_cg_render.disconnect();
		}
	}
	
	void Graph::on_render(const GUIRenderEvent& /* event */)
	{
		if(!m_cg_render)
			return;

		graph_fb->bind();
		graph_fb->clear(oe::colors::transparent);

		glm::ivec2 viewport = m_info.size;
		if(m_graph_info.graph_data.size() != 0) viewport += glm::ivec2(m_info.size.x / m_graph_info.graph_data.size(), 0.0f);
		auto& graph_renderer = GraphRenderer::getSingleton();
		graph_renderer.g_shader->bind();
		graph_renderer.g_shader->setUniform("u_graph_size", (int)m_graph_info.graph_data.size());
		graph_renderer.g_shader->setUniform("u_graph_data", m_graph_info.graph_data);
		graph_renderer.g_shader->setUniform("u_viewport", viewport);
		graph_renderer.g_shader->setUniform("u_line_w", m_graph_info.graph_line_width);
		graph_renderer.g_renderer.render();

		m_gui_manager.getWindow()->bind();

		m_graph->sprite_panel_info.sprite = &graph_fb->getSprite();
		m_graph->sprite_panel_info.color = m_graph_info.graph_color;
	}
}