#pragma once

#include "widget.hpp"



namespace oe::gui {

	typedef std::function<void(float)> slider_callback;

	struct SliderInfo {
		slider_callback callback                   = nullptr;
		float min_value                            = -1.0f;
		float max_value                            = 1.0f;
		float initial_value                        = 0.0f;
		glm::ivec2 slider_size                     = { 50, 50 };
		glm::ivec2 knob_size                       = { 50, 50 };
		glm::vec4 knob_color                       = oe::colors::grey;
		glm::vec4 slider_color                     = oe::colors::dark_grey;
		const oe::graphics::Sprite* knob_sprite    = nullptr; // must be set
		const oe::graphics::Sprite* slider_sprite  = nullptr; // must be set
		glm::vec2 offset_position                  = { 0, 0 };
		glm::vec2 align_parent                     = oe::alignments::center_center;
		glm::vec2 align_render                     = oe::alignments::center_center;
	};

	class Slider : public Widget {
	private:
		bool m_dragging;

	public:
		SliderInfo slider_info;

	public:
		Slider(const SliderInfo& slider_info);

		// Inherited via Widget
		virtual void render(oe::graphics::Renderer& renderer) override;
		virtual void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window) override;
	};

}