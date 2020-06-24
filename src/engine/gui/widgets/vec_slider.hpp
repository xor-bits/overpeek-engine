#pragma once

#include "slider.hpp"
#include <array>



namespace oe::gui
{
    struct VecSliderInfo : public SliderInfo
    {
        enum class types { columns, rows };

        std::vector<float> initial_values;
        std::vector<float> min_values;
        std::vector<float> max_values;
        types type         = types::columns;
        int slider_borders = 5;
    };

    template<size_t dimension = 3>
    class VecSlider : public Widget
    {
    private:
        std::array<Slider*, dimension> sliders;
		entt::dispatcher dispatcher{};
         
    public:
        VecSlider(VecSliderInfo slider_info)
            : Widget(slider_info.slider_size, slider_info.align_parent, slider_info.align_render, slider_info.offset_position)
        {
            switch (slider_info.type)
            {
            case VecSliderInfo::types::columns:
                slider_info.slider_size = { (slider_info.slider_size.x - (1 + dimension) * slider_info.slider_borders) / dimension, slider_info.slider_size.y };
                break;
            case VecSliderInfo::types::rows:
                slider_info.slider_size = { slider_info.slider_size.x, (slider_info.slider_size.y - (1 + dimension) * slider_info.slider_borders) / dimension };
                break;
            
            default:
                break;
            }

            if (slider_info.initial_values.size() != slider_info.min_values.size() || slider_info.max_values.size() != dimension || slider_info.min_values.size() != dimension)
            {
                __debugbreak();
                oe_error_terminate("initial_values, min_values and max_values sizes must be <dimension>");
            }
            
            glm::ivec2 offset = { 0, 0 };
            switch (slider_info.type)
            {
            case VecSliderInfo::types::columns:
                offset = glm::ivec2(slider_info.slider_size.x + slider_info.slider_borders, 0);
                break;
            case VecSliderInfo::types::rows:
                offset = glm::ivec2(0, slider_info.slider_size.y + slider_info.slider_borders);
                break;
            
            default:
                break;
            }

            slider_info.offset_position = { 0, 0 };
            slider_info.align_parent = oe::alignments::top_left;
            slider_info.align_render = oe::alignments::top_left;
            for(size_t i = 0; i < dimension; i++)
            {
                slider_info.initial_value = slider_info.initial_values[i];
                slider_info.min_value = slider_info.min_values[i];
                slider_info.max_value = slider_info.max_values[i];
                sliders[i] = new Slider(slider_info);
                addSubWidget(sliders[i]);
                slider_info.offset_position += offset;
            }
        }

        void get(std::array<float, dimension>& val) const
        { 
            for(int i = 0; i < dimension; i++) 
                val[i] = sliders[i]->slider_info.initial_value; 
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
                sliders[i]->slider_info.initial_value = val[i];
        }
    };
}