#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"

#include "engine/utility/clock.hpp"
#include <spdlog/spdlog.h>



namespace oe::gui
{
	float Widget::z_acc = 1.0f;
	Widget::Widget(const WidgetInfo& info)
		: m_parent(nullptr)
		, m_nodes() 
		, topleft_position({ 0, 0 })
		, render_position({ 0, 0 })
		, m_gui_manager(nullptr)
		, m_info(info)
	{
		z = z_acc;
		z_acc += 0.1f;
	}

	Widget::~Widget()
	{
		if (m_gui_manager) managerUnassigned(m_gui_manager);
		for (auto& w : m_nodes)
		{
			delete w;
		}
	}

	void Widget::managerAssigned(GUI* gui_manager)
	{
		m_gui_manager = gui_manager;

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&Widget::on_render>(this);
		
		for (auto& w : m_nodes)
		{
			w->managerAssigned(gui_manager);
		}
	}

	void Widget::managerUnassigned(GUI* gui_manager)
	{
		m_gui_manager = nullptr;

		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&Widget::on_render>(this);
		
		for (auto& w : m_nodes)
		{
			w->managerUnassigned(gui_manager);
		}
	}

	void Widget::setParent(Widget* parent)
	{
		m_parent = parent;
	}

	void Widget::addSubWidget(Widget* widget)
	{
		if (this == widget) { spdlog::warn("Subwidget cannot be itself"); return; }

		widget->setParent(this);
		m_nodes.insert(widget);
		if (m_gui_manager)
		{
			widget->managerAssigned(m_gui_manager);
			m_gui_manager->render_empty();
		}
	}

	void Widget::removeSubWidget(Widget* widget)
	{
		m_nodes.erase(widget);
		if (m_gui_manager)
		{
			widget->managerUnassigned(m_gui_manager);
			m_gui_manager->render_empty();
		}
	}

	void Widget::toggle(bool enabled)
	{
		m_info.toggled = enabled;
		for (auto& w : m_nodes)
		{
			w->toggle(enabled);
		}
	}

	void Widget::on_render(const GUIRenderEvent& event)
	{
		if (m_parent) render_position = m_info.offset_position + m_parent->render_position + oe::alignmentOffset(m_parent->m_info.size, m_info.align_parent) - oe::alignmentOffset(m_info.size, m_info.align_render);
		else render_position = m_info.offset_position - oe::alignmentOffset(m_info.size, m_info.align_render);
	}
}