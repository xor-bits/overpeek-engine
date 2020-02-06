#pragma once

#include "engine/graphics/interface/renderer.hpp"

#include <vector>



namespace oe::gui {

	class Widget {
	private:
		Widget* m_parent;
		std::vector<Widget*> m_nodes;
		
	public:
		glm::vec2 size;
		glm::vec2 offset_position;
		glm::vec2 topleft_position;
		glm::vec2 render_position;
		glm::vec2 align_parent = oe::alignments::center_center;
		glm::vec2 align_render = oe::alignments::center_center;

	public:
		// called from manager || __renderNodes
		void __render(oe::graphics::Renderer& renderer);
		void __resize();
		void __cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window);
		void __text(uint32_t codepoint, oe::modifiers mods);
		void __key(oe::keys key, oe::actions action, oe::modifiers mods);

		void __setParent(Widget* parent);

	public:
		Widget(const glm::vec2& size, const glm::vec2& align_parent, const glm::vec2& align_render, const glm::vec2& offset_position);
		~Widget();

		virtual void addSubWidget(Widget* widget);
		virtual void render(oe::graphics::Renderer& renderer) {}
		virtual void resize() {}
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {}
		virtual void text(uint32_t codepoint, oe::modifiers mods) {}
		virtual void key(oe::keys key, oe::actions action, oe::modifiers mods) {}

	};

}