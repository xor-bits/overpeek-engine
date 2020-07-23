#pragma once

#include "widget.hpp"
#include "engine/graphics/textLabel.hpp"



namespace oe::graphics { struct Quad; }
namespace oe::gui {

	typedef std::function<void(float)> slider_callback;

	struct SliderInfo {
		slider_callback callback                  = nullptr;
		float min_value                           = -1.0f;
		float max_value                           = 1.0f;
		float initial_value                       = 0.0f;
		bool draw_value                           = false;
		bool vertical                             = false;
		bool scroll                               = false;
		glm::ivec2 slider_size                    = { 50, 50 };
		glm::ivec2 knob_size                      = { 50, 50 };
		glm::vec4 knob_color                      = oe::colors::grey;
		glm::vec4 slider_lcolor                   = oe::colors::dark_grey;
		glm::vec4 slider_rcolor                   = oe::colors::darker_grey;
		const oe::graphics::Sprite* knob_sprite   = nullptr; // must be set
		const oe::graphics::Sprite* slider_sprite = nullptr; // must be set
		glm::ivec2 offset_position                = { 0, 0 };
		glm::vec2 align_parent                    = oe::alignments::center_center;
		glm::vec2 align_render                    = oe::alignments::center_center;
	};

	struct GUIRenderEvent;
	class GUI;
	class Slider : public Widget {
	private:
		oe::graphics::TextLabel* value_label;
		oe::graphics::Quad* label_quad;
		oe::graphics::Quad* quad_knob;
		oe::graphics::Quad* quad_lslider;
		oe::graphics::Quad* quad_rslider;

	private:
		bool m_dragging;

		void clamp();

	public:
		SliderInfo slider_info;

	public:
		Slider(const SliderInfo& slider_info);
		~Slider();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		// events
		void on_render(const GUIRenderEvent& event);
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		void on_scroll(const ScrollEvent& event);
	};

}