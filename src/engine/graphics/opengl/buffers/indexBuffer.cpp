#include "indexBuffer.hpp"

#include "engine/graphics/opengl/gl_include.hpp"



namespace oe::graphics {

	IndexBuffer::IndexBuffer(const void* data, size_t size, oe::types buffer_type)
		: Buffer(data, size, GL_ELEMENT_ARRAY_BUFFER, buffer_type == oe::types::static_type ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{}

}
