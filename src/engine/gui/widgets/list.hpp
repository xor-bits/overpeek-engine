#pragma once

#include "widget.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/interfacegen.hpp"



namespace oe::gui
{
    struct ListInfo
    {
        bool scrollable                    = true;
        int element_borders                = 5;
		glm::vec4 bg_color                 = { 0.17f, 0.17f, 0.17f, 1.0f };
        std::u32string title               = U"";
		glm::vec2 title_align              = oe::alignments::center_center;
        glm::vec4 title_bg_color           = oe::colors::darker_grey;
        int title_height                   = 0; // 28
        int title_font_size                = 16;
		const oe::graphics::Sprite* sprite = nullptr;

		WidgetInfo widget_info = { { 100, 300 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center };
    };

	class SpritePanel;
	class Slider;
    class List : public Widget
    {
    private:
        GUI* list_gui_manager;
        std::vector<Widget*> list; // max size 500
        std::vector<int> offset_accumulator; // max size 500

        oe::graphics::FrameBuffer fb;
        SpritePanel* panel;
        Slider* slider;
        oe::graphics::Sprite fb_sprite;

	public:
		ListInfo m_list_info;

	public:
		List(const ListInfo& list_info = {});
		~List();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		// this class will take ownership of this pointer
		void add(size_t pos, Widget* widget);
		void add(Widget* widget);
		void remove(size_t pos);
		void remove(Widget* widget);

		const std::vector<Widget*>& get() const { return list; }

	private:
		// events
		void on_render(const GUIRenderEvent& event);
    };
}