#include "vertexBuffer.h"

namespace graphics {

	VertexBuffer::VertexBuffer(GLfloat *data, GLsizei count, GLuint componentCount, GLenum usage) {
		mComponentCount = componentCount;
		
		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count, data, usage);
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

	void VertexBuffer::setBufferData(GLfloat *data, GLsizei count, GLuint componentCount) {
		mComponentCount = componentCount;

		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * count, data);
		unbind();
	}

}