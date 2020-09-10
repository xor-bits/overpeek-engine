#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { class Quad; }

namespace oe::gui
{
	struct TextPanelInfo
	{
		std::u32string text        = U"placeholder";
		size_t font_size           = 16;
		std::string font_path      = ""; // empty for gui default
		glm::vec4 background_color = oe::colors::transparent;
		
		WidgetInfo widget_info     = { { 0, 0 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
	};


	class TextPanel : public Widget
	{
	private:
		oe::graphics::u32TextLabel* label;
		std::shared_ptr<oe::graphics::Quad> text_quad;

	public:
		TextPanelInfo text_panel_info;

	public:
		TextPanel(const TextPanelInfo& text_panel_info);
		~TextPanel();

		virtual void managerAssigned() override;
		virtual void managerUnassigned() override;

	private:
		// events
		void on_render(const GUIRenderEvent& event);
	};
}