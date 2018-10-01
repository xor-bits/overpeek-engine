#include "vertexArray.h"

namespace graphics {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &mID);
		glBindVertexArray(mID);
	}

	VertexArray::~VertexArray() {
		for (Buffer *v : mBuffers)
		{
			delete v;
		}
		//glDeleteVertexArrays(1, &mID);
	}

	void VertexArray::addBuffer(Buffer* buffer, GLuint index) {
		bind();
		buffer->bind();

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);

		buffer->unbind();
		unbind();
	}

	void VertexArray::bind() {
		glBindVertexArray(mID);
	}

	void VertexArray::unbind() {
		glBindVertexArray(0);
	}

}