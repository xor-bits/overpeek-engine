#include "vertexArray.h"

#include "buffer.h"

#include <glad/glad.h>
#include <engine/internal_libs.h>



namespace oe::graphics {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &p_id);
		bind();
	}

	VertexArray::~VertexArray() {
		for (VertexBuffer* v : p_buffers) { delete v; }
		glDeleteVertexArrays(1, &p_id);
	}

	void VertexArray::addBuffer(VertexBuffer* buffer, unsigned int location) {
		p_buffers.push_back(buffer);

		bind();
		buffer->bind();

		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, buffer->getComponentsPerVertex(), GL_FLOAT, GL_FALSE, 0, 0);
	}

	void VertexArray::bind() {
		glBindVertexArray(p_id);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

}
