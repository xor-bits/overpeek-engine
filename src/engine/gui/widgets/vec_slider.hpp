#pragma once

#include "slider_input.hpp"
#include "engine/utility/extra.hpp"

#include <array>



namespace oe::gui
{
    enum class arrangements { columns, rows };

    template<typename ElementType = SliderInput, uint16_t dimension = 3>
    struct VecInfo
    {
        arrangements type                                          = arrangements::columns;
		int padding                                                = 3;
		int borders                                                = 0;
		bool auto_size                                             = false;
        
		std::array<typename ElementType::info_t, dimension> common = {};
		WidgetInfo                                     widget_info = {};
    };

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

    template<typename ElementType = SliderInput, uint16_t dimension = 3, bool glm_array = true>
    class Vec : public Widget
    {
	public:
		using value_t = typename VecValueTHelper<ElementType, dimension, glm_array>::value_t;
		using info_t = VecInfo<ElementType, dimension>;
		static_assert(dimension != 0, "VecSlider dimension must not be zero");
    
    public:
        std::array<std::shared_ptr<ElementType>, dimension> m_elements;
		value_t m_initial_values{};
		value_t& m_value;

	public:
		Vec(Widget* parent, GUI& gui_manager, value_t& m_value_ref, VecInfo<ElementType, dimension> vec_info = {})
            : Widget(parent, gui_manager, vec_info.widget_info)
			, m_value(m_value_ref)
        {
			const bool rows = (vec_info.type == arrangements::rows);
			if(vec_info.auto_size)
			{
				const glm::ivec2 element_size = {
					!rows * (vec_info.widget_info.size.x - 2 * vec_info.borders - (dimension - 1) * vec_info.padding) / dimension +
					 rows * (vec_info.widget_info.size.x - 2 * vec_info.borders),
					
					!rows * (vec_info.widget_info.size.y - 2 * vec_info.borders) +
					 rows * (vec_info.widget_info.size.y - 2 * vec_info.borders - (dimension - 1) * vec_info.padding) / dimension
				};
            	for(size_t i = 0; i < dimension; i++)
					vec_info.common[i].widget_info.size = element_size;
			}

            const glm::ivec2 offset_mul = {
				static_cast<int>(!rows),
				static_cast<int>(rows)
			};
            const glm::ivec2 offset_add = {
				vec_info.padding * static_cast<int>(!rows),
				vec_info.padding * static_cast<int>(rows)
			};
            const glm::ivec2 largest_element = [this, &vec_info](){
				glm::ivec2 temp = { 0, 0 };
				for(size_t i = 0; i < dimension; i++)
					temp = glm::max(temp, vec_info.common[i].widget_info.size);
				return temp;
			}();
            const glm::ivec2 size_add = {
				largest_element.x * static_cast<int>(rows),
				largest_element.y * static_cast<int>(!rows)
			};

			glm::ivec2 accumulated_offset = { vec_info.borders, vec_info.borders };
            for(size_t i = 0; i < dimension; i++)
            {
				// ref value
				typename ElementType::value_t* type_name;
				if constexpr(glm_array)
					type_name = glm::value_ptr(m_value);
				else
					type_name = m_value[0];

				// widget
            	vec_info.common[i].widget_info.align_parent = oe::alignments::top_left;
            	vec_info.common[i].widget_info.align_render = oe::alignments::top_left;
            	vec_info.common[i].widget_info.offset_position = accumulated_offset;
				m_elements[i] = create<ElementType>(*(type_name + i), vec_info.common[i]);

				// next widget offset
				if(i + 1 != dimension)
					accumulated_offset += offset_mul * vec_info.common[i].widget_info.size + offset_add;
				else
					accumulated_offset += offset_mul * vec_info.common[i].widget_info.size;
            }
			accumulated_offset += glm::ivec2{ vec_info.borders, vec_info.borders };
			m_info.size = accumulated_offset + size_add;
        }
		Vec(Widget* parent, GUI& gui_manager, VecInfo<ElementType, dimension> vec_info = {})
			: Vec(parent, gui_manager, m_initial_values, vec_info)
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