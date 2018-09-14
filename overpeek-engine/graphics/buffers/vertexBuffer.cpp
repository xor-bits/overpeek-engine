#include "vertexBuffer.h"

namespace graphics {

	VertexBuffer::VertexBuffer(GLfloat *data, GLsizei count, GLuint componentCount) {
		mComponentCount = componentCount;
		
		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count, data, GL_STATIC_DRAW);
		unbind();
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &mID);
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, mID);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}