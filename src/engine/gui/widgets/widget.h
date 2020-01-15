#pragma once

#include <vector>
#include <engine/graphics/renderer.h>



namespace oe::gui {

	class Widget {
	private:
		Widget* m_parent;
		std::vector<std::unique_ptr<Widget>> m_nodes;
		
	protected:
		oe::graphics::alignment m_align_parent;
		oe::graphics::alignment m_align_render;
		glm::vec2 m_offset_position;
		glm::vec2 m_topleft_position;
		glm::vec2 m_render_position;
		glm::vec2 m_size;

	public:
		// called from manager || __renderNodes
		void __render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer);
		void __renderNodes(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer);
		void __cursor(int button, int action, int x, int y);
		void __cursorNodes(int button, int action, int x, int y);
		void __resize();
		void __resizeNodes();

		void __setParent(Widget* parent);

	public:
		Widget(glm::ivec2 size);
		// ~Widget();

		virtual void addSubWidget(Widget* widget);
		virtual void render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) = 0;
		virtual void resize() = 0;
		virtual void cursor(int button, int action, int x, int y) = 0;

	public:
		oe::graphics::alignment& align_parent() { return m_align_parent; }
		oe::graphics::alignment& align_render() { return m_align_render; }
		glm::vec2& offset_position() { return m_offset_position; }
		glm::vec2& render_position() { return m_render_position; }
		glm::vec2& size() { return m_size; }
	};

}