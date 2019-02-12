#include "buffer.h"

#include <GL/glew.h>

namespace oe {

	Buffer::Buffer(const void *data, int count, unsigned int componentCount, int componentSize, unsigned int usage) {
		mComponentCount = componentCount;
		
		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, count * componentSize, data, usage);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &mID);
	}

	void Buffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, mID);
	}

	void Buffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Buffer::setBufferData(const void *data, unsigned int count, unsigned int componentCount, int componentSize) {
		mComponentCount = componentCount;

		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, count, data);
	}

	void *Buffer::mapBuffer() {
		bind();
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void Buffer::unmapBuffer() {
		bind();
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}