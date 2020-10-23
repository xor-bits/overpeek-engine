#include "list.hpp"

#include "sprite_panel.hpp"
#include "slider_input.hpp"
#include "text_panel.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"

#include <algorithm>



namespace oe::gui
{
    /* List::List(Widget* parent, GUI& gui_manager, const ListInfo& list_info)
        : Widget(parent, gui_manager, list_info.widget_info)
        , m_list_info(list_info)
    {
        SpritePanelInfo sprite_panel_info;
        sprite_panel_info.widget_info = list_info.widget_info;
        sprite_panel_info.sprite = list_info.sprite;
        sprite_panel_info.color = oe::colors::white;
        panel = new SpritePanel(sprite_panel_info);
        addSubWidget(panel);
        
        // Title
        if (list_info.title_height != 0) {
            SpritePanelInfo sprite_panel_info;
            sprite_panel_info.widget_info = { { m_info.size.x, list_info.title_height }, { 0, 0 }, oe::alignments::top_left, oe::alignments::top_left };
            sprite_panel_info.sprite = list_info.sprite;
            sprite_panel_info.color = list_info.title_bg_color;
            auto* title_bg = new SpritePanel(sprite_panel_info);
            panel->addSubWidget(title_bg);

            TextPanelInfo text_panel_info;
            sprite_panel_info.widget_info = { { list_info.title_font_size, list_info.title_font_size }, { 0, 0 }, list_info.title_align, list_info.title_align };
            text_panel_info.text = list_info.title;
            auto* title = new TextPanel(text_panel_info);
            title_bg->addSubWidget(title);
        }

        if (list_info.scrollable)
        {
            SliderInputInfo slider_info;
            slider_info.widget_info = { { 20, m_info.size.y }, m_info.offset_position, oe::alignments::top_right, oe::alignments::top_right, false };
            slider_info.slider_sprite = list_info.sprite;
            slider_info.knob_sprite = list_info.sprite;
            slider_info.knob_size = { 16, 16 };
            slider_info.slider_lcolor = slider_info.slider_rcolor;
            slider_info.vertical = true;
            slider_info.value_bounds = { 0.0f, 1.0f };
            slider = new SliderInput(slider_info);
            addSubWidget(slider);
        }
    }
	
	void List::virtual_toggle(bool enabled)
	{
		if(enabled)
		{
			list_gui_manager = new GUI(m_gui_manager->getWindow());
        
			FrameBufferInfo fbi;
			fbi.size = m_gui_manager->getWindow()->getSize();
			fb = oe::graphics::FrameBuffer(fbi, m_gui_manager->getWindow());
			// panel->sprite_panel_info.sprite = &fb_sprite;
        
			// event listeners
			m_gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&List::on_render>(this);
		}
		else
		{
			delete list_gui_manager;
        
			// event listeners
			m_gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&List::on_render>(this);
		}
	}

	void List::on_render(const GUIRenderEvent& event)
    {
        // TODO: fix this
        fb->bind();
        fb->clear(m_list_info.bg_color);

        list_gui_manager->render();

        m_gui_manager->getWindow()->bind();

        fb_sprite = fb->getSprite();
        fb_sprite.size = static_cast<glm::vec2>(size) / static_cast<glm::vec2>(m_gui_manager->getWindow()->getSize());
        fb_sprite.size.y *= -1.0f;

        if(offset_accumulator.size() > 0 && offset_accumulator.at(offset_accumulator.size() - 1) + m_list_info.element_borders >= size.y)
        {
            slider->toggled = true;
            float overflow_size = static_cast<float>(offset_accumulator.at(offset_accumulator.size() - 1) + m_list_info.element_borders - size.y) / static_cast<float>(size.y);
            fb_sprite.position.y -= slider->slider_info.initial_value * overflow_size;
        }
    }

    void List::add(size_t pos, Widget* widget)
    {
        widget->m_info.align_parent = oe::alignments::top_left;
        widget->m_info.align_render = oe::alignments::top_left;
        widget->m_info.offset_position = { m_list_info.element_borders, pos == 0 ? m_list_info.title_height + m_list_info.element_borders : offset_accumulator[pos - 1] };

        list.insert(list.begin() + pos, widget);
        offset_accumulator.insert(offset_accumulator.begin() + pos, static_cast<int>(widget->m_info.offset_position.y) + m_list_info.element_borders + static_cast<int>(widget->m_info.size.y));

        list_gui_manager->addSubWidget(widget);
    }

    void List::add(Widget* widget)
    {
        size_t pos = list.size();
        add(pos, widget);
    }

    void List::remove(size_t pos)
    {
        list_gui_manager->removeSubWidget(list[pos]);

        list.erase(list.begin() + pos);
        offset_accumulator.erase(offset_accumulator.begin() + pos);
    }

    void List::remove(Widget* widget)
    {
        auto iter = std::find(list.begin(), list.end(), widget);
        if (iter != list.end())
            remove(iter - list.begin());
    }
	 */
}