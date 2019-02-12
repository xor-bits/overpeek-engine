#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class QuadRenderer {
	private:
		struct TriangleVertexData
		{
			glm::vec2 position;
			glm::vec2 uv;
			GLfloat texture;
			glm::vec4 color;
		};

		Window *m_window;

		VertexArray *m_VAO;
		Buffer *m_VBO;
		IndexBuffer *m_IBO;
		GLuint m_indexcount;

		TriangleVertexData* m_buffer;
		GLuint m_buffer_current;

		//int debug = 0;

	public:
		
		//Constructor
		QuadRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit quad to renderer
		void renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color);
		
		//Draws buffers and then clears them
		void draw(GLint texture);
	};
	
}