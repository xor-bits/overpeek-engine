#include "gl_primitive_renderer.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include "engine/engine.hpp"



void oe::graphics::native_glDrawElementsPoints(size_t override_index_count)
{
	glDrawElements(GL_POINTS, override_index_count, GL_UNSIGNED_SHORT, 0);
}

void oe::graphics::native_glDrawElementsLines(size_t override_index_count)
{
	glDrawElements(GL_LINES, override_index_count, GL_UNSIGNED_SHORT, 0);
}

void oe::graphics::native_glDrawElementsTriangles(size_t override_index_count)
{
	glDrawElements(GL_TRIANGLES, override_index_count, GL_UNSIGNED_SHORT, 0);
}