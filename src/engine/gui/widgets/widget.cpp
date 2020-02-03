#include "widget.h"



namespace oe::gui {
	
	Widget::Widget(glm::ivec2 size) : 
		m_size(size), 
		m_offset_position({ 0, 0 }),
		m_render_position({ 0, 0 }),
		m_topleft_position({ 0, 0 }),
		m_nodes(std::vector<Widget*>()), 
		m_parent(nullptr), 
		m_align_parent(oe::graphics::alignment::top_left),
		m_align_render(oe::graphics::alignment::top_left)
	{

	}

	Widget::~Widget() {
		for (auto& w : m_nodes) {
			delete w;
		}
	}

	void Widget::__setParent(Widget* parent) {
		m_parent = parent;
	}

	void Widget::addSubWidget(Widget* widget) {
		widget->__setParent(this);
		m_nodes.push_back(widget);
		__resize();
	}

	void Widget::__render(oe::graphics::Renderer& renderer) {
		render(renderer);

		for (auto& w : m_nodes) {
			w->__render(renderer);
		}
	}

	void Widget::__resize() {
		if (m_parent) m_render_position = m_offset_position + m_parent->m_render_position + oe::graphics::alignmentOffset(m_parent->m_size, m_align_parent) - oe::graphics::alignmentOffset(m_size, m_align_render);
		else m_render_position = m_offset_position - oe::graphics::alignmentOffset(m_size, m_align_render);
		resize();

		for (auto& w : m_nodes) {
			w->__resize();
		}
	}

	void Widget::__cursor(int button, int action, const glm::vec2& cursor_window) {
		cursor(button, action, cursor_window);

		for (auto& w : m_nodes) {
			w->__cursor(button, action, cursor_window);
		}
	}

	void Widget::__text(unsigned int character, int mods) {
		text(character, mods);

		for (auto& w : m_nodes) {
			w->__text(character, mods);
		}
	}

	void Widget::__key(int _key, int action, int mods) {
		key(_key, action, mods);

		for (auto& w : m_nodes) {
			w->__key(_key, action, mods);
		}
	}

}