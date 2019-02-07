#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"
#include "buffers/indexBuffer.h"
#include "buffers/buffer.h"
#include "buffers/vertexArray.h"
#include "../tools/logger.h"

#define MAX_QUADS_PER_FLUSH (600000)
#define VERTEX_PER_QUAD		(4)
#define DATA_PER_VERTEX		(sizeof(TriangleVertexData) / sizeof(GLfloat))
#define INDEX_PER_QUAD		(6)
#define MAX_VBO				(MAX_QUADS_PER_FLUSH * VERTEX_PER_QUAD * DATA_PER_VERTEX)
#define MAX_IBO				(MAX_QUADS_PER_FLUSH * INDEX_PER_QUAD)

namespace graphics {

	class Window;
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

		GLuint m_framebuffer;
		GLuint m_frametexture;
		GLuint m_framebuffer2;
		GLuint m_frametexture2;


		VertexArray *m_ScreenVAO;
		Buffer *m_ScreenVBO;

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


		void renderToFramebuffer(Shader *shader, int quadTexture, int framebufferindex);
		void drawFramebuffer(Shader *postshader, int framebufferindex);
		void drawFramebufferToFramebuffer(Shader *postshader, int first, int second);
	};
	
}