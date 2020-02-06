#pragma once

#include <vector>
#include <engine/graphics/interface/renderer.h>



namespace oe::gui {

	class Widget {
	private:
		Widget* m_parent;
		std::vector<Widget*> m_nodes;
		
	protected:
		glm::vec2 m_align_parent;
		glm::vec2 m_align_render;
		glm::vec2 m_offset_position;
		glm::vec2 m_topleft_position;
		glm::vec2 m_render_position;
		glm::vec2 m_size;

	public:
		// called from manager || __renderNodes
		void __render(oe::graphics::Renderer& renderer);
		void __resize();
		void __cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window);
		void __text(uint32_t codepoint, oe::modifiers mods);
		void __key(oe::keys key, oe::actions action, oe::modifiers mods);

		void __setParent(Widget* parent);

	public:
		Widget(glm::ivec2 size);
		~Widget();

		virtual void addSubWidget(Widget* widget);
		virtual void render(oe::graphics::Renderer& renderer) {}
		virtual void resize() {}
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {}
		virtual void text(uint32_t codepoint, oe::modifiers mods) {}
		virtual void key(oe::keys key, oe::actions action, oe::modifiers mods) {}

	public:
		glm::vec2& align_parent() { return m_align_parent; }
		glm::vec2& align_render() { return m_align_render; }
		glm::vec2& offset_position() { return m_offset_position; }
		glm::vec2& render_position() { return m_render_position; }
		glm::vec2& size() { return m_size; }
	};

}