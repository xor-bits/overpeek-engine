#include "widget.h"



namespace oe::gui {
	
	Widget::Widget(glm::ivec2 size) : 
		m_size(size), 
		m_offset_position({ 0, 0 }),
		m_render_position({ 0, 0 }),
		m_topleft_position({ 0, 0 }),
		m_nodes(std::vector<std::unique_ptr<Widget>>()), 
		m_parent(nullptr), 
		m_align_parent(oe::graphics::alignment::top_left),
		m_align_render(oe::graphics::alignment::top_left)
	{

	}

	void Widget::__setParent(Widget* parent) {
		m_parent = parent;
	}

	void Widget::addSubWidget(Widget* widget) {
		widget->__setParent(this);
		m_nodes.push_back(std::unique_ptr<Widget>(widget));
		__resize();
	}

	void Widget::__render(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		render(renderer, text_renderer);

		__renderNodes(renderer, text_renderer);
	}

	void Widget::__renderNodes(oe::graphics::Renderer& renderer, oe::graphics::Renderer& text_renderer) {
		for (auto& w : m_nodes) {
			w->__render(renderer, text_renderer);
		}
	}

	void Widget::__resize() {
		if (m_parent) m_render_position = m_offset_position + m_parent->m_render_position + oe::graphics::alignmentOffset(m_parent->m_size, m_align_parent) - oe::graphics::alignmentOffset(m_size, m_align_render);
		else m_render_position = m_offset_position - oe::graphics::alignmentOffset(m_size, m_align_render);
		resize();
		spdlog::info("{} = {}, {}", typeid(*this).name(), m_render_position, m_size);

		__resizeNodes();
	}

	void Widget::__resizeNodes() {
		for (auto& w : m_nodes) {
			w->__resize();
		}
	}

	void Widget::__cursor(int button, int action, int x, int y) {
		cursor(button, action, x, y);
		__cursorNodes(button, action, x, y);
	}

	void Widget::__cursorNodes(int button, int action, int x, int y) {
		for (auto& w : m_nodes) {
			w->__cursor(button, action, x, y);
		}
	}

}