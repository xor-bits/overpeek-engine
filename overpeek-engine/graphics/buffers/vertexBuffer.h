#pragma once

#include <GL/glew.h>
#include "vertexBuffer.h"

namespace graphics {

	class VertexBuffer {
	private:
		GLuint mID;
		GLuint mComponentCount;

	public:
		VertexBuffer(GLfloat *data, GLsizei count, GLuint componentCount, GLenum usage);
		~VertexBuffer();

		void bind();
		void unbind();

		void setBufferData(GLfloat *data, GLsizei count, GLuint componentCount);

		inline GLuint getComponentCount() { return mComponentCount; }
	};

}