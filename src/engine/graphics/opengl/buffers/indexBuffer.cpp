#include "indexBuffer.hpp"

#include "../gl_renderer.hpp"

#include <glad/glad.h>



namespace oe::graphics {

	IndexBuffer::IndexBuffer(unsigned short int* data, size_t size, oe::types buffer_type)
		: Buffer(data, size, GL_ELEMENT_ARRAY_BUFFER, buffer_type == types::staticrender ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{}

}
