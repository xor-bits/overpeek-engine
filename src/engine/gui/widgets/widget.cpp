#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"

#include "engine/utility/clock.hpp"
#include "engine/utility/extra.hpp"
#include "engine/utility/connect_guard_additions.hpp"
#include "engine/graphics/interface/window.hpp"
#include <spdlog/spdlog.h>



namespace oe::gui
{
	float Widget::z_acc = 1.0f;
	Widget::Widget(Widget* parent, GUI& gui_manager, const WidgetInfo& info)
		: m_parent(parent)
		, m_nodes()
		, m_gui_manager(gui_manager)
		, z(z_acc += 0.01f)
		, topleft_position({ 0, 0 })
		, render_position({ 0, 0 })
		, m_info(info)
		, m_cg_pre_render()
	{
		toggle(true);
	}

	Widget::~Widget()
	{
		toggle(false);
	}

	void Widget::clear()
	{
		m_nodes.clear();
	}

	oe::utils::connect_guard& Widget::create_event_cg()
		{
			m_user_event_cg_guards.push_back({});
			return m_user_event_cg_guards.back();
		}

	void Widget::clear_event_cg() noexcept
	{
		m_user_event_cg_guards.clear();
	}
	
	void Widget::base_toggle(bool enabled)
	{
		if(enabled)
		{
			m_cg_pre_render.connect<GUIPreRenderEvent, &Widget::on_pre_render, Widget>(m_gui_manager.dispatcher, this);
		}
		else
		{
			m_cg_pre_render.disconnect();
		}
		virtual_toggle(enabled);
	}

	void reset_render_pos(Widget* wdgt)
	{
		if(wdgt->getParent())
			wdgt->render_position = 
			+ wdgt->m_info.offset_position
			+ wdgt->getParent()->render_position
			+ oe::alignmentOffsetRound(wdgt->getParent()->m_info.size, wdgt->m_info.align_parent)
			- oe::alignmentOffsetRound(wdgt->m_info.size, wdgt->m_info.align_render);

		else
			wdgt->render_position =
			+ wdgt->m_info.offset_position
			- oe::alignmentOffsetRound(wdgt->m_info.size, wdgt->m_info.align_render);
	}

	void Widget::addZ(float add_to_z) noexcept
	{
		z += add_to_z;
		for (auto& w : m_nodes)
			w->addZ(add_to_z);
	}

	void Widget::toggle(bool enabled)
	{
		if(m_gui_manager.getWindow()->processingEvents())
		{
			toggle_pending_value = enabled;
			toggle_pending = true;
			m_cg_pre_render.connect<GUIPreRenderEvent, &Widget::on_pre_render, Widget>(m_gui_manager.dispatcher, this);

			return;
		}
		
		{ // current node
			reset_render_pos(this);
			if(m_info.toggled != enabled)
				base_toggle(enabled);
			m_info.toggled = enabled;
		}
		{ // subnodes
			for (auto& w : m_nodes)
			{
				if(m_info.toggled == w->m_info.toggled)
					continue;
				
				w->toggle(enabled);
			}
		}
	}

	void Widget::on_pre_render(const GUIPreRenderEvent& /* event */)
	{
		if(!m_cg_pre_render)
			return;

		if(toggle_pending)
		{
			toggle(toggle_pending_value);
			toggle_pending = false;
		}

		reset_render_pos(this);
	}
}