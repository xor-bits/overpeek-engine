#include "gl.hpp"

#include "gl_include.hpp"



uint32_t oe::graphics::gl::GL_POINT_ENUM = GL_POINTS;
uint32_t oe::graphics::gl::GL_LINE_ENUM = GL_LINES;
uint32_t oe::graphics::gl::GL_TRIANGLE_ENUM = GL_TRIANGLES;
uint32_t oe::graphics::gl::GL_INT8_ENUM = GL_BYTE;
uint32_t oe::graphics::gl::GL_UINT8_ENUM = GL_UNSIGNED_BYTE;
uint32_t oe::graphics::gl::GL_INT16_ENUM = GL_SHORT;
uint32_t oe::graphics::gl::GL_UINT16_ENUM = GL_UNSIGNED_SHORT;
uint32_t oe::graphics::gl::GL_INT32_ENUM = GL_INT;
uint32_t oe::graphics::gl::GL_UINT32_ENUM = GL_UNSIGNED_INT;

void oe::graphics::gl::nglDrawElements(uint32_t primitive, int32_t offset, uint32_t type, int32_t count)
{
	glDrawElements(primitive, std::abs(count), type, reinterpret_cast<void*>(std::abs(offset) * sizeof(uint16_t)));
}