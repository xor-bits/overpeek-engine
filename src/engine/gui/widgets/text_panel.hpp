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

			// visuals
			oe::utils::color_string<char32_t> text = { U"placeholder", oe::colors::white };
			TextOptions               text_options = {};
			// base
			Widget::Info               widget_info = {};
		};
		using info_t = Info;
		
	private:
		std::unique_ptr<oe::graphics::TextLabel> label;
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