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
		// event listeners
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Widget::on_render>(this);
	}

	Widget::~Widget() {
		for (auto& w : m_nodes) {
			delete w;
		}

		// event listeners
		m_gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Widget::on_render>(this);
	}

	void Widget::setParent(Widget* parent) {
		m_parent = parent;
	}

	void Widget::addSubWidget(Widget* widget) {
		widget->setParent(this);
		m_nodes.push_back(widget);
		m_gui_manager->render_empty();
	}

	void Widget::on_render(const GUIRenderEvent& event) {
		if (m_parent) render_position = offset_position + m_parent->render_position + oe::alignmentOffset(m_parent->size, align_parent) - oe::alignmentOffset(size, align_render);
		else render_position = offset_position - oe::alignmentOffset(size, align_render);
	}

}