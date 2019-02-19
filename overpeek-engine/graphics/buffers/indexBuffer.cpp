#include "indexBuffer.h"

#include <GL/glew.h>

namespace oe {

	IndexBuffer::IndexBuffer(unsigned short int *data, int count, unsigned int usage) {
		mCount = count;

		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short int) * count, data, usage);
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

	void IndexBuffer::setBufferData(const void *data, int count) {
		mCount = count;

		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short int) * count, data);
	}

}