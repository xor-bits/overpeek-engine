#pragma once

#include "slider.hpp"
#include <array>



namespace oe::gui
{
    struct VecSliderInfo : public SliderInfo
    {
        enum class types { columns, rows };

        types type = types::columns;
        int slider_borders = 5;
    };

    template<size_t dimension = 3>
    class VecSlider : public Widget
    {
    private:
        std::array<Slider*, dimension> sliders;
         
    public:
        VecSlider(VecSliderInfo slider_info)
        {
            glm::ivec2 offset = { 0, 0 };
            for(size_t i = 0; i < dimension; i++)
            {
                slider_info.offset_position += offset;
                sliders[i] = new Slider(slider_info);

                switch (slider_info.type)
                {
                case types::columns:
                    offset = { slider_info.slider_size.x, 0 };
                    break;
                case types::rows:
                    offset = { 0, slider_info.slider_size.y };
                    break;
                
                default:
                    break;
                }
            }
        }

    };
}