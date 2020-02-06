#include "widget.hpp"



namespace oe::gui {
	
	Widget::Widget(glm::ivec2 size) : 
		m_size(size), 
		m_offset_position({ 0, 0 }),
		m_render_position({ 0, 0 }),
		m_topleft_position({ 0, 0 }),
		m_nodes(std::vector<Widget*>()), 
		m_parent(nullptr), 
		m_align_parent(oe::alignments::top_left),
		m_align_render(oe::alignments::top_left)
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
		if (m_parent) m_render_position = m_offset_position + m_parent->m_render_position + oe::alignmentOffset(m_parent->m_size, m_align_parent) - oe::alignmentOffset(m_size, m_align_render);
		else m_render_position = m_offset_position - oe::alignmentOffset(m_size, m_align_render);
		resize();

		for (auto& w : m_nodes) {
			w->__resize();
		}
	}

	void Widget::__cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) {
		cursor(button, action, cursor_window);

		for (auto& w : m_nodes) {
			w->__cursor(button, action, cursor_window);
		}
	}

	void Widget::__text(uint32_t codepoint, oe::modifiers mods) {
		text(codepoint, mods);

		for (auto& w : m_nodes) {
			w->__text(codepoint, mods);
		}
	}

	void Widget::__key(oe::keys _key, oe::actions _action, oe::modifiers _mods) {
		key(_key, _action, _mods);

		for (auto& w : m_nodes) {
			w->__key(_key, _action, _mods);
		}
	}

}