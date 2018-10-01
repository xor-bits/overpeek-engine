#pragma once

#include <GL/glew.h>

namespace graphics {

	class IndexBuffer {
	private:
		GLuint mID;
		GLuint mCount;

	public:
		IndexBuffer(GLushort *data, GLsizei count);
		~IndexBuffer();

		void bind();
		void unbind();

		inline GLuint getCount() { return mCount; }
	};

}