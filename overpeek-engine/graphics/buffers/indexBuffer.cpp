#include "indexBuffer.h"

namespace graphics {

	IndexBuffer::IndexBuffer(GLushort *data, GLsizei count) {
		mCount = count;

		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * count, data, GL_STATIC_DRAW);
		unbind();
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

}