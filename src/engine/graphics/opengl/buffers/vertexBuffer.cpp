#include "vertexBuffer.h"

#include "engine/graphics/opengl/gl_renderer.h"
#include "engine/internal_libs.h"

#include <glad/glad.h>



namespace oe::graphics {

	VertexBuffer::VertexBuffer(const void* data, size_t size, unsigned int components_per_vertex, oe::types buffer_type)
		: Buffer(data, size, GL_ARRAY_BUFFER, buffer_type == types::staticrender ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{
		p_components_per_vertex = components_per_vertex;
	}

	void VertexBuffer::attrib(int index, int components, size_t startBytes) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, components, GL_FLOAT, false, p_components_per_vertex * sizeof(float), (void*)startBytes);
	}

}
