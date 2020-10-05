#pragma once

#include "slider.hpp"
#include "engine/utility/extra.hpp"

#include <array>



namespace oe::gui
{
    enum class arrangements { columns, rows };

    template<glm::length_t dimension = 3>
    struct VecSliderInfo
    {
        arrangements type                         = arrangements::columns;
		int slider_borders                        = 3;
        glm::vec<dimension, float> initial_values = glm::vec<dimension, float>(0.0f);
        glm::vec<dimension, float> min_values     = glm::vec<dimension, float>(-1.0f);
        glm::vec<dimension, float> max_values     = glm::vec<dimension, float>(1.0f);

        SliderInfo slider_info;
    };

    template<glm::length_t dimension = 3>
	struct VecSliderHoverEvent
	{};

    template<glm::length_t dimension = 3>
	struct VecSliderUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		glm::vec<dimension, float> value;
	};

    template<glm::length_t dimension = 3>
    class VecSlider : public Widget
    {
    protected:
        std::array<std::shared_ptr<Slider>, dimension> m_sliders;
		VecSliderHoverEvent<dimension> m_event_hover_latest;
		VecSliderUseEvent<dimension> m_event_use_latest;
         
    public:
		glm::vec<dimension, float> m_initial_values;
		glm::vec<dimension, float>& m_value;

	public:
		VecSlider(Widget* parent, GUI& gui_manager, glm::vec<dimension, float>& m_value_ref, VecSliderInfo<dimension> slider_info = {})
            : Widget(parent, gui_manager, slider_info.slider_info.widget_info)
			, m_value(m_value_ref)
        {
			static_assert(dimension != 0, "VecSlider dimension must not be zero");

            glm::ivec2 offset_next = { 0, 0 };
            switch (slider_info.type)
            {
            case arrangements::columns:
                slider_info.slider_info.widget_info.size = { (slider_info.slider_info.widget_info.size.x - (dimension - 1) * slider_info.slider_borders) / dimension, slider_info.slider_info.widget_info.size.y };
                offset_next = glm::ivec2(slider_info.slider_info.widget_info.size.x + slider_info.slider_borders, 0);
				m_info.size = {
					slider_info.slider_info.widget_info.size.x * dimension + (dimension - 1) * slider_info.slider_borders,
					slider_info.slider_info.widget_info.size.y
				};
                break;
            case arrangements::rows:
                slider_info.slider_info.widget_info.size = { slider_info.slider_info.widget_info.size.x, (slider_info.slider_info.widget_info.size.y - (dimension - 1) * slider_info.slider_borders) / dimension };
                offset_next = glm::ivec2(0, slider_info.slider_info.widget_info.size.y + slider_info.slider_borders);
				m_info.size = {
					slider_info.slider_info.widget_info.size.x,
					slider_info.slider_info.widget_info.size.y * dimension + (dimension - 1) * slider_info.slider_borders
				};
                break;
            default:
                break;
            }


			size_t min_w_or_h = std::min(slider_info.slider_info.widget_info.size.x, slider_info.slider_info.widget_info.size.y);
			slider_info.slider_info.knob_size = { min_w_or_h, min_w_or_h };

            slider_info.slider_info.widget_info = { slider_info.slider_info.widget_info.size, { 0, 0 }, oe::alignments::top_left, oe::alignments::top_left };
            for(glm::length_t i = 0; i < dimension; i++)
            {
                slider_info.slider_info.value_bounds = { slider_info.min_values[i], slider_info.max_values[i] };
				m_sliders[i] = create<Slider>(*(&m_value[0] + i), slider_info.slider_info);
                slider_info.slider_info.widget_info.offset_position += offset_next;

				m_sliders[i]->template connect_listener<SliderHoverEvent, &VecSlider<dimension>::on_slider_hover>(this);
				m_sliders[i]->template connect_listener<SliderUseEvent, &VecSlider<dimension>::on_slider_use>(this);
            }
        }
		VecSlider(Widget* parent, GUI& gui_manager, VecSliderInfo<dimension> slider_info = {})
			: VecSlider(parent, gui_manager, m_initial_values, slider_info)
		{ m_initial_values = slider_info.initial_values; }

		~VecSlider()
		{
			for(size_t i = 0; i < dimension; i++)
            {
				m_sliders[i]->template disconnect_listener<SliderHoverEvent, &VecSlider<dimension>::on_slider_hover>(this);
				m_sliders[i]->template disconnect_listener<SliderUseEvent, &VecSlider<dimension>::on_slider_use>(this);
            }
		}

	private:
		void on_slider_hover(const SliderHoverEvent& e)
		{
			dispatcher.trigger(m_event_hover_latest);
		}

		void on_slider_use(const SliderUseEvent& e)
		{
			m_event_use_latest.action = e.action;
			m_event_use_latest.modifier = e.modifier;
			m_event_use_latest.button = e.button;
			m_event_use_latest.value = m_value;
			
			dispatcher.trigger(m_event_use_latest);
		}
    };
}