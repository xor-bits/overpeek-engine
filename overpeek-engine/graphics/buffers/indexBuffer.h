#pragma once

#include <GL/glew.h>

namespace oe {

	class IndexBuffer {
	private:
		GLuint mID;
		GLuint mCount;

	public:
		IndexBuffer(GLushort *data, GLsizei count, GLenum usage);
		~IndexBuffer();

		void bind();
		void unbind();

		void setBufferData(const GLvoid *data, GLsizei count);

		inline GLuint getCount() { return mCount; }
	};

}