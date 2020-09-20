#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"

#include "engine/utility/clock.hpp"
#include "engine/utility/extra.hpp"
#include "engine/graphics/interface/window.hpp"
#include <spdlog/spdlog.h>



namespace oe::gui
{
	float Widget::z_acc = 1.0f;
	Widget::Widget(Widget* parent, GUI& gui_manager, const WidgetInfo& info)
		: m_parent(parent)
		, m_nodes()
		, topleft_position({ 0, 0 })
		, render_position({ 0, 0 })
		, m_gui_manager(gui_manager)
		, m_info(info)
		, z(z_acc += 0.01f)
		, m_cg_pre_render()
	{
		m_cg_pre_render = { m_gui_manager.dispatcher, this };
	}

	Widget::~Widget()
	{
		toggle(false);
	}
	
	void Widget::base_toggle(bool enabled)
	{
		if(enabled)
		{
			m_cg_pre_render = { m_gui_manager.dispatcher, this };
		}
		else
		{
			m_cg_pre_render.reset();
		}
		virtual_toggle(enabled);
	}

	void Widget::toggle(bool enabled)
	{
		if(m_gui_manager.getWindow()->processingEvents())
		{
			toggle_pending_value = enabled;
			toggle_pending = true;
			m_cg_pre_render = { m_gui_manager.dispatcher, this };

			return;
		}

		{ // current node
			if(m_info.toggled != enabled)
				base_toggle(enabled);
		}
		{ // subnodes
			m_info.toggled = enabled;
			for (auto& w : m_nodes)
			{
				if(m_info.toggled == w->m_info.toggled)
					continue;
				
				w->toggle(enabled);
			}
		}
	}

	void Widget::on_pre_render(const GUIPreRenderEvent& event)
	{
		if(toggle_pending)
		{
			toggle(toggle_pending_value);
			toggle_pending = false;
		}

		if(m_parent)
			render_position = 
			+ m_info.offset_position
			+ m_parent->render_position
			+ oe::alignmentOffset(m_parent->m_info.size, m_info.align_parent)
			- oe::alignmentOffset(m_info.size, m_info.align_render);

		else
			render_position =
			+ m_info.offset_position
			- oe::alignmentOffset(m_info.size, m_info.align_render);
	}
}