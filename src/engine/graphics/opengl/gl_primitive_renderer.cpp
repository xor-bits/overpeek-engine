#include "gl_primitive_renderer.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "engine/engine.hpp"



void oe::graphics::native_glDrawElementsPoints(const int32_t override_index_offset, const int32_t override_index_count)
{
	glDrawElements(GL_POINTS, std::abs(override_index_count), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(std::abs(override_index_offset) * sizeof(uint16_t)));
}

void oe::graphics::native_glDrawElementsLines(const int32_t override_index_offset, const int32_t override_index_count)
{
	glDrawElements(GL_LINES, std::abs(override_index_count), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(std::abs(override_index_offset) * sizeof(uint16_t)));
}

void oe::graphics::native_glDrawElementsTriangles(const int32_t override_index_offset, const int32_t override_index_count)
{
	glDrawElements(GL_TRIANGLES, std::abs(override_index_count), GL_UNSIGNED_SHORT, reinterpret_cast<void*>(std::abs(override_index_offset) * sizeof(uint16_t)));
}