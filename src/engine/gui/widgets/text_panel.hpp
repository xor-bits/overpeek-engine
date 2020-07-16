#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { class Quad; }
namespace oe::gui
{
	struct TextPanelInfo
	{
		std::u32string text        = U"";
		int font_size              = 16;
		glm::vec4 background_color = oe::colors::transparent;
		glm::ivec2 offset_position = { 0, 0 };
		glm::vec2 align_parent     = oe::alignments::center_center;
		glm::vec2 align_render     = oe::alignments::center_center;
	};

	struct GUIRenderEvent;
	class GUI;
	class TextPanel : public Widget
	{
	private:
		oe::graphics::u32TextLabel* label;
		oe::graphics::Quad* text_quad;

	public:
		TextPanelInfo text_panel_info;

	public:
		TextPanel(const TextPanelInfo& text_panel_info);
		~TextPanel();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		// events
		void on_render(const GUIRenderEvent& event);
	};

}