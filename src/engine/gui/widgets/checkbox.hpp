#pragma once

#include "widget.hpp"
#include "button.hpp"



namespace oe::gui {

#define CHECKBOX_CALLBACK_WRAPPER(x) [&](bool toggled) { x(toggled); }
	typedef std::function<void(bool toggled)> checkbox_callback;

	struct CheckboxInfo {
		checkbox_callback callback              = nullptr;
		bool initial                            = false;
		glm::ivec2 size                         = { 24, 24 };
		glm::vec2 offset_position               = { 0, 0 };
		glm::vec2 align_parent                  = oe::alignments::center_center;
		glm::vec2 align_render                  = oe::alignments::center_center;
		glm::vec4 color_back                    = oe::colors::dark_grey;
		glm::vec4 color_mark                    = oe::colors::lime;
		const oe::graphics::Sprite* sprite      = nullptr; // must be set
	};

	struct GUIRenderEvent;
	class GUI;
	class Checkbox : public Widget {
	private:
		oe::graphics::Quad* quad_check;
		oe::graphics::Quad* quad_box; // hehe

	public:
		CheckboxInfo m_checkbox_info;
		oe::gui::Button* m_button;

		void checkbox_action(oe::mouse_buttons button, oe::actions action);

	public:
		Checkbox(GUI* gui_manager, const CheckboxInfo& checkbox_info);
		~Checkbox();

		// events
		void on_render(const GUIRenderEvent& event);
	};

}