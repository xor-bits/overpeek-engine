#include "graph.hpp"

#include "engine/gui/gui_manager.hpp"
#include "engine/asset/asset_loader.hpp"
#include "engine/graphics/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/random.hpp"
#include "engine/utility/connect_guard_additions.hpp"



namespace oe::gui
{
	class GraphRenderer
	{
	private:
		static GraphRenderer* singleton;
		GraphRenderer(const GraphRenderer& copy) = delete;
		GraphRenderer()
			: g_shader({ "asset:graph_shader", {
					{
						oe::shader_stages::vertex_shader,
						oe::asset::AssetLoader::get().resource_string("shader/default_shader/shader.vert.glsl"),
						{}
					},
					{
						oe::shader_stages::fragment_shader,
						oe::asset::AssetLoader::get().resource_string("shader/gui/graph.frag.glsl"),
						{}
					}
				}})
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
		constexpr static oe::RasterizerInfo g_raster = { oe::modes::disable, oe::depth_functions::always, oe::culling_modes::neither, oe::polygon_mode::fill, 1.0f, 1.0f };
	};
	GraphRenderer* GraphRenderer::singleton = nullptr;



    Graph::Graph(Widget* parent, GUI& gui_manager, const info_t& info)
        : SpritePanel(parent, gui_manager, info.bg_panel_info)
        , m_graph_info(info)
    {
		SpritePanel::info_t spi = m_graph_info.bg_panel_info;
		spi.color_tint = oe::colors::transparent;
		spi.widget_info.pixel_size = { 0, 0 };
		spi.widget_info.fract_size = { 1.0f, 1.0f };
		spi.widget_info.pixel_origon_offset = { 0, 0 };
		spi.widget_info.fract_origon_offset = oe::alignments::top_left;
		spi.widget_info.fract_render_offset = oe::alignments::top_left;
		spi.sprite = nullptr;
		m_graph = create(spi);
    }
	
	void Graph::virtual_toggle(bool enabled)
	{
		SpritePanel::virtual_toggle(enabled);
		if(enabled)
		{
			graph_fb = oe::graphics::FrameBuffer({ m_render_size });

			// event listeners
			m_cg_render.connect<GUIRenderEvent, &Graph::on_render, Graph>(m_gui_manager.m_dispatcher, this);
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


		auto& graph_renderer = GraphRenderer::getSingleton();
		auto& engine = oe::Engine::getSingleton();
		
		// rasterizer settings
		const oe::RasterizerInfo old_rasterizer_info = engine.getRasterizerInfo();
		engine.setRasterizerInfo(graph_renderer.g_raster);

		// bind fb
		graph_fb->bind();
		graph_fb->clear(oe::colors::transparent);
		// draw
		glm::ivec2 viewport = m_render_size;
		if(m_graph_info.graph_data.size() != 0) viewport += glm::ivec2(m_render_size.x / m_graph_info.graph_data.size(), 0.0f);
		graph_renderer.g_shader->bind();
		graph_renderer.g_shader->setUniform("u_graph_size", (int)m_graph_info.graph_data.size());
		graph_renderer.g_shader->setUniform("u_graph_data", m_graph_info.graph_data);
		graph_renderer.g_shader->setUniform("u_viewport", viewport);
		graph_renderer.g_shader->setUniform("u_line_w", m_graph_info.graph_line_width);
		graph_renderer.g_renderer.render();
		// unbind fb
		m_gui_manager.getWindow()->bind();

		// back to original rasterizer settings
		engine.setRasterizerInfo(old_rasterizer_info);

		m_graph->sprite_panel_info.sprite = &graph_fb->getSprite();
		m_graph->sprite_panel_info.color_tint = m_graph_info.graph_color;
	}
}