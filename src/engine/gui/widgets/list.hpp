#pragma once

#include "widget.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/interfacegen.hpp"



namespace oe::gui
{
    struct ListInfo
    {
		glm::ivec2 size                    = { 100, 300 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::top_left;
		glm::vec2 align_render             = oe::alignments::top_left;
        bool scrollable                    = true;
        int element_borders                = 5;
		glm::vec4 bg_color                 = { 0.17f, 0.17f, 0.17f, 1.0f };
        std::wstring title                 = L"";
		glm::vec2 title_align              = oe::alignments::center_center;
        glm::vec4 title_bg_color           = oe::colors::darker_grey;
        int title_height                   = 0; // 28
        int title_font_size                = 16;
		const oe::graphics::Sprite* sprite = nullptr; // must be set
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
		List(const ListInfo& list_info);
		~List();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		// events
		void on_render(const GUIRenderEvent& event);

		// this class will take ownership of this pointer
		void add(size_t pos, Widget* widget);
		void add(Widget* widget);
		void remove(size_t pos);
		void remove(Widget* widget);

		const std::vector<Widget*>& get() const { return list; }

    };
}