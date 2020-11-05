#include "button_decorated.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/connect_guard_additions.hpp"

#include "text_panel.hpp"
#include "sprite_panel.hpp"
#include "button.hpp"



namespace oe::gui
{
	DecoratedButton::DecoratedButton(Widget* parent, GUI& gui_manager, const info_t& _button_info)
		: Button(parent, gui_manager, _button_info.button_info)
		, button_info(_button_info)
	{
		SpritePanel::info_t sp_info;
		sp_info.widget_info.size = m_info.size;
		sp_info.widget_info.align_parent = oe::alignments::center_center;
		sp_info.widget_info.align_render = oe::alignments::center_center;
		sp_info.sprite = button_info.sprite;
		sp_info.color_tint = button_info.btn_color;
		button_background = create(sp_info);

		TextPanel::info_t tp_info;
		tp_info.text = button_info.text;
		tp_info.font_size = _button_info.text_font_size;
		tp_info.widget_info.align_parent = oe::alignments::center_center;
		tp_info.widget_info.align_render = oe::alignments::center_center;
		button_text = create(tp_info);
	}
	
	void DecoratedButton::virtual_toggle(bool enabled)
	{
		Button::virtual_toggle(enabled);
		if(enabled)
		{
			m_cg_render.connect<GUIRenderEvent, &DecoratedButton::on_render, DecoratedButton>(m_gui_manager.m_dispatcher, this);
		}
		else
		{
			m_cg_render.disconnect();
		}
	}

	void DecoratedButton::on_render(const GUIRenderEvent& /* event */)
	{
		if (!button_info.autoresize || !m_cg_render)
			return;

		const glm::ivec2 new_size = button_text->m_info.size + button_info.padding * 2;
		button_background->m_info.size = new_size;
		m_info.size = new_size;
	}

}