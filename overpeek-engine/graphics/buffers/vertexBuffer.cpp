#include "vertexBuffer.h"

#include "../renderer.h"

#include "../../internal_libs.h"

#include <GL/glew.h>

namespace oe {

	VertexBuffer::VertexBuffer(const void* data, size_t size, unsigned int components_per_vertex, int buffer_type)
		: Buffer(data, size, GL_ARRAY_BUFFER, buffer_type == types::staticrender ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{
		p_components_per_vertex = components_per_vertex;
	}

	void VertexBuffer::attrib(int index, int components, size_t startBytes) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, components, GL_FLOAT, false, p_components_per_vertex * sizeof(float), (void*)startBytes);
	}

}