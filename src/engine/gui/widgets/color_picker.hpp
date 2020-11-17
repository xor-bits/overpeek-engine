#pragma once

#include "color_input.hpp"
#include "sprite_panel.hpp"
#include "engine/graphics/vertexData.hpp"

#include "engine/interfacegen.hpp"



namespace oe::gui
{
	template<typename T>
	struct BasicSliderInputHoverEvent;
	template<typename T>
	struct BasicSliderInputUseEvent;
	template<typename T>
	class BasicSliderInput;



	struct ColorPickerHoverEvent : public BaseHoverEvent {};
	struct ColorPickerUseEvent : public BaseUseEvent
	{
		oe::color value;
	};



	class ColorPicker : public SpritePanel
	{
	public:
		using value_t = oe::color;
		using hover_event_t = ColorPickerHoverEvent;
		using use_event_t = ColorPickerUseEvent;
		struct info_t
		{
			using widget_t = ColorPicker;

			bool preview                        = true;
			bool alpha                          = true;
			ColorInput::info_t color_input_info = { oe::colors::red, 2, 14, {}, oe::colors::dark_grey, nullptr, input_type::slider, false, close_fn::never, open_fn::never, { { 200, 225 }, { 0, 0 }, oe::alignments::center_center, oe::alignments::center_center } };
		};
	
	private:
		oe::graphics::FrameBuffer m_wheel_fb;

		bool m_dragging_wheel = false;
		bool m_dragging_triangle = false;
		float m_direction = 0.0f;
		std::array<oe::graphics::VertexData, 3> m_triangle_vertices;
		glm::vec3 m_barycentric_pos_triangle;
		glm::ivec2 m_selector_wheel;
		glm::ivec2 m_selector_triangle;
		value_t m_value_last;

		std::shared_ptr<BasicSliderInput<float>> m_alpha_slider;
		std::shared_ptr<SpritePanel> m_framebuffer_panel;
		std::shared_ptr<SpritePanel> m_preview;

		constexpr static float m_wheel_width = 0.1f;
		constexpr static float m_triangle_width = 1.0f - m_wheel_width * 2.5f;
		constexpr static float m_equilateral_triangle_angles = 2.0f * glm::pi<float>() / 3.0f;
		constexpr static float m_triangle_width_2 = m_triangle_width * m_triangle_width;
		
		// sqrt(r^2 + r^2 - 2r^2cos(a))
		// = sqrt(2r^2() - 2r^2cos(a))
		// = sqrt(2r^2(1-cos(a)))
		constexpr static float m_triangle_line_width = gcem::sqrt<float>(2.0f * m_triangle_width_2 * (1.0f - gcem::cos(m_equilateral_triangle_angles * 2.0f)));

	public:
		info_t m_color_picker_info;
		value_t& m_value;
		ColorPickerHoverEvent m_event_hover_latest;
		ColorPickerUseEvent m_event_use_latest;

	public:
		ColorPicker(Widget* parent, GUI& gui_manager, const info_t& color_picker_info, value_t& m_value_ref);
		ColorPicker(Widget* parent, GUI& gui_manager, const info_t& color_picker_info)
			: ColorPicker(parent, gui_manager, color_picker_info, m_color_picker_info.color_input_info.initial_color)
		{}
		~ColorPicker() override;

		virtual void virtual_toggle(bool enabled) override;
	
	private:
		void update_from_value(bool update_dir);
		void update_to_value();
		// events
		void on_render(const GUIRenderEvent& event);
		void on_cursor(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		oe::utils::connect_guard m_cg_render;
		oe::utils::connect_guard m_cg_cursor;
		oe::utils::connect_guard m_cg_button;
		void on_slider_use(const BasicSliderInputUseEvent<float>& event);
	};
}