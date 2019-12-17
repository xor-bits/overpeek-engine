#include "indexBuffer.h"

#include "../renderer.h"

#include <GL/glew.h>



namespace oe::graphics {

	IndexBuffer::IndexBuffer(unsigned short int* data, size_t size, int buffer_type)
		: Buffer(data, size, GL_ELEMENT_ARRAY_BUFFER, buffer_type == types::staticrender ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{}

}