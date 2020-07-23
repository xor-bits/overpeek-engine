#pragma once

#include "engine/graphics/interface/renderer.hpp"

#include <vector>



namespace oe::gui {

	struct GUIRenderEvent;
	class GUI;
	class Widget {
	private:
		Widget* m_parent;
		std::unordered_set<Widget*> m_nodes;
		void setParent(Widget* parent);
		static float z_acc;

	protected:
		float z;

	public:
		glm::ivec2 size = { 0, 0 };
		glm::ivec2 offset_position = { 0, 0 };
		glm::ivec2 topleft_position = { 0, 0 };
		glm::ivec2 render_position = { 0, 0 };
		glm::vec2 align_parent = oe::alignments::center_center;
		glm::vec2 align_render = oe::alignments::center_center;
		bool toggled = true;
		
		GUI* m_gui_manager;

	public:
		Widget(const glm::ivec2& size, const glm::vec2& align_parent, const glm::vec2& align_render, const glm::ivec2& offset_position);
		virtual ~Widget();

		// unassign before reassigning
		virtual void managerAssigned(GUI* gui_manager);
		virtual void managerUnassigned(GUI* gui_manager);

		// events
		void on_render(const GUIRenderEvent& event);

		// this class will take ownership of this pointer
		virtual void addSubWidget(Widget* widget);
		virtual void removeSubWidget(Widget* widget);

	};

}