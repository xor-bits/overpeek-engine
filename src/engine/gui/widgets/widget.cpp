#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"



namespace oe::gui {
	
	Widget::Widget(const glm::ivec2& _size, const glm::vec2& _align_parent, const glm::vec2& _align_render, const glm::ivec2& _offset_position)
		: size(_size)
		, align_parent(_align_parent)
		, align_render(_align_render)
		, offset_position(_offset_position)
		, render_position({ 0, 0 })
		, topleft_position({ 0, 0 })
		, m_nodes() 
		, m_parent(nullptr)
		, m_gui_manager(nullptr)
	{}

	Widget::~Widget() {
		for (auto& w : m_nodes)
		{
			delete w;
		}
		if (m_gui_manager) managerUnassigned(m_gui_manager);
	}

	void Widget::managerAssigned(GUI* gui_manager)
	{
		m_gui_manager = gui_manager;
		
		for (auto& w : m_nodes)
		{
			w->managerAssigned(gui_manager);
		}

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Widget::on_render>(this);
	}

	void Widget::managerUnassigned(GUI* gui_manager)
	{
		m_gui_manager = nullptr;
		
		for (auto& w : m_nodes)
		{
			w->managerUnassigned(gui_manager);
		}

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Widget::on_render>(this);
	}

	void Widget::setParent(Widget* parent) {
		m_parent = parent;
	}

	void Widget::addSubWidget(Widget* widget) {
		widget->setParent(this);
		m_nodes.insert(widget);
		if (m_gui_manager)
		{
			widget->managerAssigned(m_gui_manager);
			m_gui_manager->render_empty();
		}
	}

	void Widget::removeSubWidget(Widget* widget) {
		m_nodes.erase(widget);
		if (m_gui_manager)
		{
			widget->managerUnassigned(m_gui_manager);
			m_gui_manager->render_empty();
		}
	}

	void Widget::on_render(const GUIRenderEvent& event) {
		if (m_parent) render_position = offset_position + m_parent->render_position + oe::alignmentOffset(m_parent->size, align_parent) - oe::alignmentOffset(size, align_render);
		else render_position = offset_position - oe::alignmentOffset(size, align_render);
	}

}