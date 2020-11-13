#pragma once

#include <cstdint>
#include "engine/enum.hpp"



namespace oe::graphics::gl
{
	extern uint32_t GL_POINT_ENUM;
	extern uint32_t GL_LINE_ENUM;
	extern uint32_t GL_TRIANGLE_ENUM;
	extern uint32_t GL_INT8_ENUM;
	extern uint32_t GL_UINT8_ENUM;
	extern uint32_t GL_INT16_ENUM;
	extern uint32_t GL_UINT16_ENUM;
	extern uint32_t GL_INT32_ENUM;
	extern uint32_t GL_UINT32_ENUM;

	extern void nglDrawElements(uint32_t primitive, int32_t offset, uint32_t type, int32_t count);
	
	
	
	template<oe::primitive_types primitive>
	constexpr uint32_t& getGLPrimitive()
	{
		if constexpr(primitive == oe::primitive_types::points)
			return GL_POINT_ENUM;
		else if constexpr(primitive == oe::primitive_types::lines)
			return GL_LINE_ENUM;
		else
			return GL_TRIANGLE_ENUM;
	}

	template<typename index_T>
	constexpr uint32_t& getGLIndexT()
	{
		if constexpr(std::is_same_v<index_T, int8_t>)
			return GL_INT8_ENUM;
		else if constexpr(std::is_same_v<index_T, uint8_t>)
			return GL_UINT8_ENUM;
		if constexpr(std::is_same_v<index_T, int16_t>)
			return GL_INT16_ENUM;
		else if constexpr(std::is_same_v<index_T, uint16_t>)
			return GL_UINT16_ENUM;
		if constexpr(std::is_same_v<index_T, int32_t>)
			return GL_INT32_ENUM;
		else /* if constexpr(std::is_same_v<index_T, uint32_t>) */
			return GL_UINT32_ENUM;
	}
	
	template<oe::primitive_types primitive, typename index_T>
	void drawElements(int32_t offset, int32_t count)
	{
		nglDrawElements(getGLPrimitive<primitive>(), offset, getGLIndexT<index_T>(), count);
	}
}