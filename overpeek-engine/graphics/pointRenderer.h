#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

///TODO: make use of geometry shader program

namespace oe {

	class VertexArray;
	class Buffer;
	class IndexBuffer;
	class Window;

	class PointRenderer {
	private:
		struct PointVertexData
		{
			glm::vec2 position;
			glm::vec2 size;
			GLfloat texture;
			glm::vec4 color;
		};

		VertexArray *m_vao;
		Buffer *m_vbo;
		GLuint pointcount;

		Window *m_window;

		PointVertexData* m_buffer;
		GLuint m_buffer_current;
	
	public:

		//Initialize particle rendering
		PointRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit point to renderer
		void renderPoint(glm::vec2 pos, glm::vec2 size, int id, glm::vec4 color);

		//Draws buffers and then clears them
		void draw(int pointtexture);
	
	};

}