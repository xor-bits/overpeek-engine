#include "widget.hpp"
#include "engine/gui/gui_manager.hpp"

#include "engine/utility/clock.hpp"
#include "engine/utility/extra.hpp"
#include "engine/utility/connect_guard_additions.hpp"
#include "engine/graphics/interface/window.hpp"



namespace oe::gui
{
	Widget::Widget(Widget* parent, GUI& gui_manager, const Info& info)
		: m_parent(parent)
		, m_nodes()
		, m_gui_manager(gui_manager)
		, m_z(s_z_acc += 0.01f)
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
			m_cg_pre_render.connect<GUIPreRenderEvent, &Widget::on_pre_render, Widget>(m_gui_manager.m_dispatcher, this);
		else
			m_cg_pre_render.disconnect();
		virtual_toggle(enabled);
	}

	void Widget::reset_render_pos()
	{
		const glm::ivec2 parent_pos = getParent() ? getParent()->m_render_position : glm::ivec2{ oe::gui::border, oe::gui::border };
		const glm::ivec2 parent_size = getParent() ? getParent()->m_render_size : m_gui_manager.getWindow()->getSize();
		const glm::ivec2 fract_pos = static_cast<glm::ivec2>(static_cast<glm::vec2>(parent_size) * m_info.fract_origon_offset);
		const glm::ivec2 fract_size = static_cast<glm::ivec2>(static_cast<glm::vec2>(parent_size) * m_info.fract_size);

		m_render_size =
			+ m_info.pixel_size
			+ fract_size;
		
		m_render_position =
			+ m_info.pixel_origon_offset
			+ parent_pos
			+ oe::alignmentOffsetRound(parent_size, m_info.fract_origon_offset)
			- oe::alignmentOffsetRound(m_render_size, m_info.align_origon);
	}

	void Widget::addZ(float add_to_z) noexcept
	{
		m_z += add_to_z;
		for (auto& w : m_nodes)
			w->addZ(add_to_z);
	}

	void Widget::toggle(bool enabled)
	{
		if(m_gui_manager.getWindow()->processingEvents())
		{
			m_toggle_pending_value = enabled;
			m_toggle_pending = true;
			m_cg_pre_render.connect<GUIPreRenderEvent, &Widget::on_pre_render, Widget>(m_gui_manager.m_dispatcher, this);

			return;
		}
		
		{ // current node
			reset_render_pos();
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

		if(m_toggle_pending)
		{
			toggle(m_toggle_pending_value);
			m_toggle_pending = false;
		}

		reset_render_pos();
	}
}