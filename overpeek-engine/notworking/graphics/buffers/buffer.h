#pragma once

#include <GL/glew.h>

namespace graphics {

	class Buffer {
	private:
		GLuint mID;
		GLuint mComponentCount;

	public:
		Buffer(GLfloat *data, GLsizei count, GLuint componentCount, GLenum usage);
		~Buffer();

		void bind();
		void unbind();

		void setBufferData(GLfloat *data, GLsizei count, GLuint componentCount);

		inline GLuint getComponentCount() { return mComponentCount; }
	};

}