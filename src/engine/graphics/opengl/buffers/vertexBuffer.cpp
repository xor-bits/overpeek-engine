#include "vertexBuffer.hpp"

#include "engine/internal_libs.hpp"

#include "engine/graphics/opengl/gl_include.hpp"



namespace oe::graphics {

	VertexBuffer::VertexBuffer(const void* data, size_t size, unsigned int components_per_vertex, oe::types buffer_type)
		: Buffer(data, size, GL_ARRAY_BUFFER, buffer_type == types::static_type ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW)
	{
		p_components_per_vertex = components_per_vertex;
	}

	void VertexBuffer::attrib(int index, int components, size_t startBytes) const {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, components, GL_FLOAT, false, p_components_per_vertex * sizeof(float), (void*)startBytes);
	}

}
