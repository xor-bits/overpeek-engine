#pragma once

#include "color_picker.hpp"
#include "sprite_panel.hpp"
#include "engine/graphics/vertexData.hpp"

#include "engine/interfacegen.hpp"



namespace oe::gui
{
	class Slider;

	struct ColorPickerWheelInfo
	{
		bool preview                      = true;
		bool alpha                        = true;
		ColorPickerInfo color_picker_info = { 2, oe::colors::red, oe::colors::dark_grey, nullptr, false, close_fn::never, open_fn::never, { { 200, 225 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center } };
	};

	class ColorPickerWheel : public SpritePanel
	{
	private:
		oe::graphics::FrameBuffer wheel_fb;

		bool dragging_wheel = false;
		bool dragging_triangle = false;
		float direction = 0.0f;
		std::array<oe::graphics::VertexData, 3> triangle_vertices;
		glm::vec3 barycentric_pos_triangle;
		glm::ivec2 selector_wheel;
		glm::ivec2 selector_triangle;

		Slider* m_alpha_slider;
		SpritePanel* m_framebuffer_panel;
		SpritePanel* m_preview;

		constexpr static float wheel_width = 0.1f;
		constexpr static float triangle_width = 1.0f - wheel_width * 2.5f;
		constexpr static float equilateral_triangle_angles = 2.0f * glm::pi<float>() / 3.0f;
		constexpr static float triangle_width_2 = triangle_width * triangle_width;
		
		// sqrt(r^2 + r^2 - 2r^2cos(a))
		// = sqrt(2r^2() - 2r^2cos(a))
		// = sqrt(2r^2(1-cos(a)))
		constexpr static float triangle_line_width = gcem::sqrt<float>(2.0f * triangle_width_2 * (1.0f - gcem::cos(equilateral_triangle_angles * 2.0f)));

	public:
		ColorPickerWheelInfo m_color_picker_info;
		ColorPickerHoverEvent m_event_hover_latest;
		ColorPickerUseEvent m_event_use_latest;

	public:
		ColorPickerWheel(const ColorPickerWheelInfo& color_picker_info = {});
		~ColorPickerWheel();

		virtual void managerAssigned(GUI* gui_manager) override;
		virtual void managerUnassigned(GUI* gui_manager) override;

		const glm::vec4& get() const;
		void set(const glm::vec4& color);
		void update();

	private:
		// events
		void on_render(const GUIRenderEvent& event);
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		void on_slider_use(const SliderUseEvent& event);
	};
}