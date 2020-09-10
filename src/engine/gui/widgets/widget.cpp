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
		toggle(false);
		for (auto& w : m_nodes)
		{
			delete w;
		}
	}

	void Widget::managerAssigned()
	{
		m_gui_manager->dispatcher.sink<GUIPreRenderEvent>().connect<&Widget::on_pre_render>(this);
	}

	void Widget::managerUnassigned()
	{
		m_gui_manager->dispatcher.sink<GUIPreRenderEvent>().disconnect<&Widget::on_pre_render>(this);
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

		if (!m_gui_manager) return;

		std::function<void(Widget*, GUI*)> lambda;
		lambda = [&](Widget* root, GUI* gui_manager)
		{
			if (root->m_info.toggled && root->m_gui_manager != m_gui_manager && m_gui_manager != nullptr)
			{
				root->m_gui_manager = m_gui_manager;
				root->managerAssigned();
			}

			for (auto& w : m_nodes)
				lambda(w, gui_manager);
		};

		lambda(this, m_gui_manager);
	}

	void Widget::removeSubWidget(Widget* widget)
	{
		m_nodes.erase(widget);
	}

	void Widget::toggle(bool enabled)
	{
		if (!m_gui_manager)
		{
			oe_error_terminate("crash");
			spdlog::critical("cannot toggle non-assigned widget");
			return;
		}

		std::function<void(Widget*,GUI*)> lambda;
		lambda = [&](Widget* root, GUI* gui_manager)
		{
			if (root->m_info.toggled)
			{
				if(root->m_gui_manager != m_gui_manager && root->m_gui_manager != nullptr)
					root->managerUnassigned();

				if (root->m_gui_manager != m_gui_manager && m_gui_manager != nullptr)
				{
					root->m_gui_manager = m_gui_manager;
					root->managerAssigned();
				}
			}

			for (auto& w : m_nodes)
				lambda(w, gui_manager);
		};

		lambda(this, enabled ? m_gui_manager : nullptr);
	}

	void Widget::on_pre_render(const GUIPreRenderEvent& event)
	{
		if (m_parent) render_position = m_info.offset_position + m_parent->render_position + oe::alignmentOffset(m_parent->m_info.size, m_info.align_parent) - oe::alignmentOffset(m_info.size, m_info.align_render);
		else render_position = m_info.offset_position - oe::alignmentOffset(m_info.size, m_info.align_render);
	}
}