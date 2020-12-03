#pragma once

#include "slider_input.hpp"
#include "engine/utility/extra.hpp"

#include <array>



namespace oe::gui
{
	template<typename ElementType = SliderInput, uint16_t dimension = 3, bool glm_array = true>
	struct VecValueTHelper;
	
	template<typename ElementType, uint16_t dimension>
	struct VecValueTHelper<ElementType, dimension, true>
	{
		using value_t = glm::vec<dimension, typename ElementType::value_t>;
	};
	
	template<typename ElementType, uint16_t dimension>
	struct VecValueTHelper<ElementType, dimension, false>
	{
		using value_t = std::array<typename ElementType::value_t, dimension>;
	};



    enum class arrangements { columns, rows };

    template<typename ElementType, uint16_t dimension = 3, bool glm_array = true>
    class Vec : public Widget
    {
	public:
		using value_t = typename VecValueTHelper<ElementType, dimension, glm_array>::value_t;
		using element_t = ElementType;
		using element_info_t = typename element_t::info_t;
		struct Info
		{
			using widget_t = Vec;
			
			// info for each element
			std::array<element_info_t, dimension> common = {};
			// info for the whole
			arrangements                            type = arrangements::columns;
			int                                  padding = 3;
			int                                  borders = 0;
			Widget::Info                     widget_info = {};
		};
		using info_t = Info;

		static_assert(dimension != 0, "VecSlider dimension must not be zero");
		static_assert(dimension, "VecSlider dimension must me in the range of i32");
    
    public:
        std::array<std::shared_ptr<ElementType>, dimension> m_elements;
		value_t m_initial_values{};
		value_t& m_value;

	public:
		Vec(Widget* parent, GUI& gui_manager, info_t vec_info, value_t& m_value_ref)
            : Widget(parent, gui_manager, vec_info.widget_info)
			, m_value(m_value_ref)
        {
			const bool rows = (vec_info.type == arrangements::rows);

			/* const glm::ivec2 element_pixel_size = {
				!rows * (vec_info.widget_info.pixel_size.x - 2 * vec_info.borders - (dimension - 1) * vec_info.padding) / dimension +
				rows * (vec_info.widget_info.pixel_size.x - 2 * vec_info.borders),
				
				!rows * (vec_info.widget_info.pixel_size.y - 2 * vec_info.borders) +
				rows * (vec_info.widget_info.pixel_size.y - 2 * vec_info.borders - (dimension - 1) * vec_info.padding) / dimension
			}; */

			const glm::vec2 element_fract_size = {
				!rows * 1.0f / static_cast<float>(dimension + 1) +
				rows * 1.0f,
				
				!rows * 1.0f +
				rows * 1.0f / static_cast<float>(dimension + 1),
			};

            const glm::ivec2 offset_mul_i = {
				static_cast<int>(!rows),
				static_cast<int>(rows)
			};

            const glm::vec2 offset_mul_f = static_cast<glm::vec2>(offset_mul_i);

            /* const glm::ivec2 offset_add = {
				vec_info.padding * static_cast<int>(!rows),
				vec_info.padding * static_cast<int>(rows)
			}; */

            /* const glm::ivec2 largest_element = [&vec_info](){
				glm::ivec2 temp = { 0, 0 };
				for(uint16_t i = 0; i < dimension; i++)
					temp = glm::max(temp, vec_info.common[i].widget_info.pixel_size);
				return temp;
			}(); */

            /* const glm::ivec2 size_add = {
				largest_element.x * static_cast<int>(rows),
				largest_element.y * static_cast<int>(!rows)
			}; */

            for(uint16_t i = 0; i < dimension; i++)
            {
				// ref value
				typename ElementType::value_t* type_name;
				if constexpr(glm_array)
					type_name = glm::value_ptr(m_value);
				else
					type_name = m_value.data();

				// widget
				vec_info.common[i].widget_info.pixel_size = { 0, 0 };
            	vec_info.common[i].widget_info.fract_size = element_fract_size;
				vec_info.common[i].widget_info.pixel_origon_offset = glm::ivec2(vec_info.borders) + static_cast<int>(i) * offset_mul_i * glm::ivec2(vec_info.padding);
            	vec_info.common[i].widget_info.fract_origon_offset = (static_cast<float>(i) * offset_mul_f * element_fract_size);
				m_elements[i] = create(vec_info.common[i], *(type_name + i));
            }
        }
		Vec(Widget* parent, GUI& gui_manager, const info_t& vec_info)
			: Vec(parent, gui_manager, vec_info, m_initial_values)
		{
			if constexpr(glm_array)
            	for(size_t i = 0; i < dimension; i++)
					*(glm::value_ptr(m_initial_values) + i) = vec_info.common[i].initial_value;
			else
				m_initial_values.fill(vec_info.common.value_initial);
		}

		[[nodiscard]] constexpr std::shared_ptr<ElementType>& operator[](size_t i) const noexcept
		{
			return m_elements[i];
		}
    };
}
