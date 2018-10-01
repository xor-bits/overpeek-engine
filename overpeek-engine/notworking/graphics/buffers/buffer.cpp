#include "buffer.h"

namespace graphics {

	Buffer::Buffer(GLfloat *data, GLsizei count, GLuint componentCount, GLenum usage) {
		mComponentCount = componentCount;
		
		glGenBuffers(1, &mID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count, data, usage);
		unbind();
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

	void Buffer::setBufferData(GLfloat *data, GLsizei count, GLuint componentCount) {
		mComponentCount = componentCount;

		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * count, data);
		unbind();
	}

}