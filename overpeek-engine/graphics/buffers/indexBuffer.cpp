#include "indexBuffer.h"

namespace oe {

	IndexBuffer::IndexBuffer(GLushort *data, GLsizei count, GLenum usage) {
		mCount = count;

		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * count, data, usage);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &mID);
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setBufferData(const GLvoid *data, GLsizei count) {
		mCount = count;

		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * count, data);
	}

}