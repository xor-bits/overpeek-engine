#pragma once

#include "widget.hpp"
#include "sprite_panel.hpp"
#include "engine/interfacegen.hpp"

#include <gsl/span> // update to C++20 standard as when atleast msvc, g++ and clang++ support it fully



namespace oe::gui
{
    class Graph : public SpritePanel
    {
    public:
		using value_t = void;
		struct info_t
		{
			using widget_t = Graph;

			// value
			gsl::span<float>       graph_data = {};
			// visuals
			float            graph_line_width = 1.0f;
			oe::color             graph_color = oe::colors::green;
			// base
			SpritePanel::info_t bg_panel_info = {};
		};
		
    public:
        info_t m_graph_info;

    private:
        std::shared_ptr<SpritePanel> m_graph;
        oe::graphics::FrameBuffer graph_fb;

    public:
        Graph(Widget* parent, GUI& gui_manager, const info_t& info);

		void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
    };
}