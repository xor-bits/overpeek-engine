#pragma once

#include "slider.hpp"
#include "engine/utility/extra.hpp"

#include <array>



namespace oe::gui
{
    enum class arrangements { columns, rows };

    template<size_t dimension = 3>
    struct VecSliderInfo : public SliderInfo
    {
        arrangements type = arrangements::columns;
		int slider_borders = 3;
        glm::vec<dimension, float> initial_values;
        glm::vec<dimension, float> min_values;
        glm::vec<dimension, float> max_values;
    };

    template<size_t dimension = 3>
	struct VecSliderHoverEvent
	{};

    template<size_t dimension = 3>
	struct VecSliderUseEvent
	{
		oe::actions action;
		oe::mouse_buttons button;
		oe::modifiers modifier;
		glm::vec<dimension, float> value;
	};

    template<size_t dimension = 3>
    class VecSlider : public Widget
    {
    protected:
        std::array<Slider*, dimension> sliders;
		VecSliderHoverEvent<dimension> event_hover_latest;
		VecSliderUseEvent<dimension> event_use_latest;
         
    public:
        VecSlider(VecSliderInfo<dimension> slider_info)
            : Widget(slider_info.slider_size, slider_info.align_parent, slider_info.align_render, slider_info.offset_position)
        {
			static_assert(dimension != 0, "VecSlider dimension must not be zero");

            glm::ivec2 offset_next = { 0, 0 };
            switch (slider_info.type)
            {
            case arrangements::columns:
                slider_info.slider_size = { (slider_info.slider_size.x - (dimension - 1) * slider_info.slider_borders) / dimension, slider_info.slider_size.y };
                offset_next = glm::ivec2(slider_info.slider_size.x + slider_info.slider_borders, 0);
				size = {
					slider_info.slider_size.x * dimension + (dimension - 1) * slider_info.slider_borders,
					slider_info.slider_size.y
				};
                break;
            case arrangements::rows:
                slider_info.slider_size = { slider_info.slider_size.x, (slider_info.slider_size.y - (dimension - 1) * slider_info.slider_borders) / dimension };
                offset_next = glm::ivec2(0, slider_info.slider_size.y + slider_info.slider_borders);
				size = {
					slider_info.slider_size.x,
					slider_info.slider_size.y * dimension + (dimension - 1) * slider_info.slider_borders
				};
                break;
            default:
                break;
            }


			size_t min_w_or_h = std::min(slider_info.slider_size.x, slider_info.slider_size.y);
			slider_info.knob_size = { min_w_or_h, min_w_or_h };

            slider_info.offset_position = { 0, 0 };
            slider_info.align_parent = oe::alignments::top_left;
            slider_info.align_render = oe::alignments::top_left;
            for(size_t i = 0; i < dimension; i++)
            {
                slider_info.value_initial = slider_info.initial_values[i];
                slider_info.value_bounds = { slider_info.min_values[i], slider_info.max_values[i] };
                sliders[i] = new Slider(slider_info);
                addSubWidget(sliders[i]);
                slider_info.offset_position += offset_next;

				sliders[i]->connect_listener<SliderHoverEvent, &VecSlider<dimension>::on_slider_hover>(this);
				sliders[i]->connect_listener<SliderUseEvent, &VecSlider<dimension>::on_slider_use>(this);
            }
        }

		~VecSlider()
		{
			for(size_t i = 0; i < dimension; i++)
            {
				sliders[i]->disconnect_listener<SliderHoverEvent, &VecSlider<dimension>::on_slider_hover>(this);
				sliders[i]->disconnect_listener<SliderUseEvent, &VecSlider<dimension>::on_slider_use>(this);
            }
		}

        void get(std::array<float, dimension>& val) const
        { 
            for(int i = 0; i < dimension; i++) 
                val[i] = sliders[i]->slider_info.value_initial; 
        }

        std::array<float, dimension> get() const
        {
            std::array<float, dimension> l;
            get(l);
            return l;
        }

        glm::vec<dimension, float> getGLM() const
        {
            auto list = get();
            return oe::utils::listToVec<dimension, float>(&(*list.begin()));
        }

        void set(const std::array<float, dimension>& val)
        {
            for(int i = 0; i < dimension; i++)
                sliders[i]->slider_info.value_initial = val[i];
        }

	private:
		void on_slider_hover(const SliderHoverEvent& e)
		{
			dispatcher.trigger(event_hover_latest);
		}

		void on_slider_use(const SliderUseEvent& e)
		{
			event_use_latest.action = e.action;
			event_use_latest.modifier = e.modifier;
			event_use_latest.button = e.button;
			event_use_latest.value = getGLM();
			
			dispatcher.trigger(event_use_latest);
		}
    };
}