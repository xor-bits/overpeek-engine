#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {
	
	Widget::Widget(GUI* gui_manager, const glm::vec2& _size, const glm::vec2& _align_parent, const glm::vec2& _align_render, const glm::vec2& _offset_position)
		: size(_size)
		, align_parent(_align_parent)
		, align_render(_align_render)
		, offset_position(_offset_position)
		, render_position({ 0, 0 })
		, topleft_position({ 0, 0 })
		, m_nodes(std::vector<Widget*>()) 
		, m_parent(nullptr)
		, m_gui_manager(gui_manager)
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
		m_gui_manager->render_empty();
	}

	void Widget::__render(float& z, oe::graphics::Renderer* renderer) {
		z += 1.0f;
		render(z, renderer);

		for (auto& w : m_nodes) {
			z += 1.0f;
			w->__render(z, renderer);
		}
	}

	void Widget::__resize() {
		if (m_parent) render_position = offset_position + m_parent->render_position + oe::alignmentOffset(m_parent->size, align_parent) - oe::alignmentOffset(size, align_render);
		else render_position = offset_position - oe::alignmentOffset(size, align_render);
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