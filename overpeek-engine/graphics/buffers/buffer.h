#pragma once

#include <GL/glew.h>

namespace graphics {

	class Buffer {
	private:
		GLuint mID;
		GLuint mComponentCount;

	public:
		/*
		- "count" means all of the objects in the array
		- "componentCount" is amount of components per vertex
		- "componentSize" is the size of one object in bytes
		- "usage" is either GL_STATIC_DRAW or GL_DYNAMIC_DRAW
		*/
		Buffer(const GLvoid *data, GLsizei count, GLuint componentCount, GLsizei componentSize, GLenum usage);
		~Buffer();

		void bind();
		void unbind();

		void setBufferData(const GLvoid *data, GLsizei count, GLuint componentCount, GLsizei componentSize);
		void *mapBuffer();
		void unmapBuffer();

		inline GLuint getComponentCount() { return mComponentCount; }
	};

}