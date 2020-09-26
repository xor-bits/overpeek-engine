#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	struct TextPanelInfo
	{
		oe::graphics::text_render_input<char32_t> text = U"placeholder";
		uint16_t font_size                             = 16;
		std::string font_path                          = ""; // empty for gui default
		glm::vec4 background_color                     = oe::colors::transparent;
	
		WidgetInfo widget_info                         = { { 0, 0 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};


	class TextPanel : public Widget
	{
	private:
		oe::graphics::u32TextLabel* label;
		std::unique_ptr<oe::graphics::Quad> text_quad;

	public:
		TextPanelInfo text_panel_info;

	public:
		TextPanel(Widget* parent, GUI& gui_manager, const TextPanelInfo& text_panel_info = {});
		~TextPanel();

	public:
		virtual void virtual_toggle(bool enabled) override;
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
	};
}