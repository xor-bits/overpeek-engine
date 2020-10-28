#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"
#include "engine/utility/fileio.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	class TextPanel : public Widget
	{
	public:
		struct Info
		{
			using widget_t = TextPanel;

			
			oe::graphics::text_render_input<char32_t> text = U"placeholder";
			uint16_t font_size                             = 16;
			oe::utils::FontFile font_file                  = {}; // empty for gui default
			glm::vec4 background_color                     = oe::colors::transparent;
		
			Widget::Info widget_info                       = { { 0, 0 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
		};
		using info_t = Info;
		
	private:
		oe::graphics::u32TextLabel* label;
		std::unique_ptr<oe::graphics::Quad> text_quad;

	public:
		info_t text_panel_info;

	public:
		TextPanel(Widget* parent, GUI& gui_manager, const info_t& text_panel_info);

	public:
		void virtual_toggle(bool enabled) override;
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
	};
}