#include "graph.hpp"

#include "engine/assets/default_shader/shader.vert.hpp"

constexpr char graph_shader_frag[] = R"glsl(
#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) out vec4 color;
		
in vec2 shader_uv_frag;
in vec4 shader_color_frag;

void main()
{
    color = vec4(1.0, 0.5, 0.0, 1.0);
}
)glsl";


namespace oe::gui
{
    Graph::Graph(const GraphInfo& info)
        : Widget(info.bg_panel_info.widget_info)
        , m_graph_info(info)
        , bg(info.bg_panel_info)
    {
        
    }

	void Graph::managerAssigned(GUI* gui_manager)
	{
		graph_fb = oe::graphics::FrameBuffer({ m_info.size });

		Widget::managerAssigned(gui_manager);
	}

	void Graph::managerUnassigned(GUI* gui_manager)
	{
		graph_fb.reset();

		Widget::managerUnassigned(gui_manager);
	}
}