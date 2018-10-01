#pragma once

#include <GL/glew.h>

namespace graphics {

	class Buffer {
	private:
		GLuint mID;
		GLuint mComponentCount;

	public:
		Buffer(const GLvoid *data, GLsizei count, GLuint componentCount, GLsizei componentSize, GLenum usage);
		~Buffer();

		void bind();
		void unbind();

		void setBufferData(const GLvoid *data, GLsizei count, GLuint componentCount, GLsizei componentSize);

		inline GLuint getComponentCount() { return mComponentCount; }
	};

}