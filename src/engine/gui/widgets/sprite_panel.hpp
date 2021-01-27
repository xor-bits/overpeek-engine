#pragma once

#include "widget.hpp"
#include "engine/graphics/primitives.hpp"
#include "engine/enum.hpp"



namespace oe::graphics { class Quad; struct Sprite; }

namespace oe::gui
{	
	class SpritePanel : public Widget
	{
	public:
		using value_t = void;
		struct info_t
		{
			using widget_t = SpritePanel;
			
			// visuals
			oe::color               color_tint = oe::colors::white;
			const oe::graphics::Sprite* sprite = nullptr;
			float                     rotation = 0.0f;
			glm::vec2       rotation_alignment = oe::alignments::center_center;
			// base
			Widget::Info           widget_info = {};
		};
		
	private:
		std::unique_ptr<oe::graphics::Quad> quad;
		
	public:
		info_t sprite_panel_info;

	public:
		SpritePanel(Widget* parent, GUI& gui_manager, const info_t& sprite_panel_info);

		void virtual_toggle(bool enabled) override;
	
	private:
		// events
		void on_render(const GUIRenderEvent& event);
		oe::utils::connect_guard m_cg_render;
	};

}
