#pragma once

#include "widget.hpp"
#include "sprite_panel.hpp"
#include "engine/interfacegen.hpp"

#include <gsl/span> // update to C++20 standard as when atleast msvc, g++ and clang++ support it fully



namespace oe::gui
{
    struct GraphInfo
    {
        gsl::span<float> graph_data;
		float graph_line_width      = 1.0f;
        glm::vec4 graph_color       = oe::colors::green;

        SpritePanelInfo bg_panel_info = { oe::colors::translucent_black, nullptr, false, { { 300, 200 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center } };
    };

    class Graph : public SpritePanel
    {
    public:
        GraphInfo m_graph_info;

    private:
        SpritePanel m_graph;
        oe::graphics::FrameBuffer graph_fb;

    public:
        Graph(const GraphInfo& info);

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

	private:
		// events
		void on_render(const GUIRenderEvent& event);
    };
}