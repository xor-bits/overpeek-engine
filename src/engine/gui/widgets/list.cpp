#include "list.hpp"

#include "sprite_panel.hpp"
#include "slider.hpp"
#include "text_panel.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/graphics/interface/window.hpp"

#include <algorithm>



namespace oe::gui
{
    List::List(const ListInfo& list_info)
        : Widget(list_info.size, list_info.align_parent, list_info.align_render, list_info.offset_position)
        , m_list_info(list_info)
    {
        SpritePanelInfo sprite_panel_info;
        sprite_panel_info.size = list_info.size;
        sprite_panel_info.offset_position = list_info.offset_position;
        sprite_panel_info.align_parent = list_info.align_parent;
        sprite_panel_info.align_render = list_info.align_render;
        sprite_panel_info.sprite = list_info.sprite;
        sprite_panel_info.color = oe::colors::white;
        panel = new SpritePanel(sprite_panel_info);
        addSubWidget(panel);
        
        // Title
        if (list_info.title_height != 0) {
            SpritePanelInfo sprite_panel_info;
            sprite_panel_info.size = { list_info.size.x, list_info.title_height };
            sprite_panel_info.align_parent = oe::alignments::top_left;
            sprite_panel_info.align_render = oe::alignments::top_left;
            sprite_panel_info.sprite = list_info.sprite;
            sprite_panel_info.color = list_info.title_bg_color;
            auto* title_bg = new SpritePanel(sprite_panel_info);
            panel->addSubWidget(title_bg);

            TextPanelInfo text_panel_info;
            text_panel_info.font_size = list_info.title_font_size;
            text_panel_info.align_parent = list_info.title_align;
            text_panel_info.align_render = list_info.title_align;
            text_panel_info.text = list_info.title;
            auto* title = new TextPanel(text_panel_info);
            title_bg->addSubWidget(title);
        }

        if (list_info.scrollable)
        {
            SliderInfo slider_info;
            slider_info.slider_size = { 20, list_info.size.y };
            slider_info.offset_position = list_info.offset_position;
            slider_info.align_parent = oe::alignments::top_right;
            slider_info.align_render = oe::alignments::top_right;
            slider_info.slider_sprite = list_info.sprite;
            slider_info.knob_sprite = list_info.sprite;
            slider_info.knob_size = { 16, 16 };
            slider_info.slider_lcolor = slider_info.slider_rcolor;
            slider_info.vertical = true;
            slider_info.min_value = 0.0f;
            slider_info.max_value = 1.0f;
            slider = new Slider(slider_info);
            slider->toggled = false;
            addSubWidget(slider);
        }
    }

    List::~List()
    {}

	void List::managerAssigned(GUI* gui_manager)
	{
        list_gui_manager = new GUI(gui_manager->getWindow());
        
        FrameBufferInfo fbi;
        fbi.size = gui_manager->getWindow()->getSize();
        fb = oe::graphics::FrameBuffer(fbi, gui_manager->getWindow());
        panel->sprite_panel_info.sprite = &fb_sprite;
        
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().connect<&List::on_render>(this);

		Widget::managerAssigned(gui_manager);
	}

	void List::managerUnassigned(GUI* gui_manager)
	{
        delete list_gui_manager;
        
		// event listeners
		gui_manager->dispatcher.sink<GUIRenderEvent>().disconnect<&List::on_render>(this);

		Widget::managerUnassigned(gui_manager);
	}

	void List::on_render(const GUIRenderEvent& event)
    {
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
        widget->align_parent = oe::alignments::top_left;
        widget->align_render = oe::alignments::top_left;
        widget->offset_position.x = m_list_info.element_borders;
        widget->offset_position.y = pos == 0 ? m_list_info.title_height + m_list_info.element_borders : offset_accumulator[pos - 1];

        list.insert(list.begin() + pos, widget);
        offset_accumulator.insert(offset_accumulator.begin() + pos, static_cast<int>(widget->offset_position.y) + m_list_info.element_borders + static_cast<int>(widget->size.y));

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
}