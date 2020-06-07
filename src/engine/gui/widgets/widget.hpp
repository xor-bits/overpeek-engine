#pragma once

#include "engine/graphics/interface/renderer.hpp"

#include <vector>



namespace oe::gui {

	struct GUIRenderEvent;
	class GUI;
	class Widget {
	private:
		Widget* m_parent;
		std::vector<Widget*> m_nodes;
		void setParent(Widget* parent);

	public:
		glm::vec2 size;
		glm::vec2 offset_position;
		glm::vec2 topleft_position;
		glm::vec2 render_position;
		glm::vec2 align_parent = oe::alignments::center_center;
		glm::vec2 align_render = oe::alignments::center_center;
		
		GUI* m_gui_manager;

	public:
		Widget(GUI* gui_manager, const glm::vec2& size, const glm::vec2& align_parent, const glm::vec2& align_render, const glm::vec2& offset_position);
		virtual ~Widget();

		// events
		void on_render(const GUIRenderEvent& event);

		virtual void addSubWidget(Widget* widget);

	};

}