#pragma once

#include <GL/glew.h>
#include "vertexBuffer.h"

namespace graphics {

	class VertexBuffer {
	private:
		GLuint mID;
		GLuint mComponentCount;

	public:
		VertexBuffer(GLfloat *data, GLsizei count, GLuint componentCount);
		~VertexBuffer();

		void bind();
		void unbind();

		inline GLuint getComponentCount() { return mComponentCount; }
	};

}