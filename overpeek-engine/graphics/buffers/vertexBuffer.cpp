#include "vertexBuffer.h"

#include "../renderer.h"
#include "../../utility/logger.h"

#include <GL/glew.h>

namespace oe {

	VertexBuffer::VertexBuffer(const void* data, size_t size, unsigned int components_per_vertex, int buffer_type)
		: Buffer(data, size, GL_ARRAY_BUFFER, buffer_type == types::staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{
		p_components_per_vertex = components_per_vertex;
	}

	void VertexBuffer::attrib(int index, int components, int strideBytes, int startBytes) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, components, GL_FLOAT, false, strideBytes, (void*)startBytes);
	}

}