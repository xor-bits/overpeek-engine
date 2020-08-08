#pragma once

#include "widget.hpp"



namespace oe::graphics { struct Quad; struct Sprite; }
namespace oe::gui { class Button; struct ButtonUseEvent; struct ButtonHoverEvent; }

namespace oe::gui
{
	struct CheckboxInfo
	{
		glm::ivec2 size                    = { 24, 24 };
		glm::ivec2 offset_position         = { 0, 0 };
		glm::vec2 align_parent             = oe::alignments::center_center;
		glm::vec2 align_render             = oe::alignments::center_center;

		bool initial                       = false;

		glm::vec4 color_back               = oe::colors::dark_grey;
		glm::vec4 color_mark               = oe::colors::lime;

		const oe::graphics::Sprite* sprite = nullptr; // must be set
	};

	struct CheckboxHoverEvent
	{};

	struct CheckboxUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		bool value;
	};

	class Checkbox : public Widget {
	private:
		std::shared_ptr<oe::graphics::Quad> quad_check;
		std::shared_ptr<oe::graphics::Quad> quad_box; // hehe
		oe::gui::Button* m_button;

	public:
		CheckboxInfo m_checkbox_info;
		CheckboxHoverEvent event_hover_latest;
		CheckboxUseEvent event_use_latest;

	public:
		Checkbox(const CheckboxInfo& checkbox_info);
		~Checkbox();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_button_use(const ButtonUseEvent& e);
		void on_button_hover(const ButtonHoverEvent& e);
	};

}