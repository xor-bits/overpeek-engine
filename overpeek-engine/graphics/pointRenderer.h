#pragma once

#include "shader.h"
#include "buffers/indexBuffer.h"
#include "buffers/buffer.h"
#include "buffers/vertexArray.h"
#include "../tools/logger.h"
#include "window.h"

#define MAX_POINTS_PER_FLUSH 64000
#define MAX_VBO MAX_POINTS_PER_FLUSH * 2


///TODO: make use of geometry shader program

namespace graphics {

	class PointRenderer {
	private:
		Window *m_window;

		VertexArray *m_vao;
		Buffer *m_vbo;
		Buffer *m_id;
		Buffer *m_color;

		GLuint pointcount;
		GLfloat m_vertex_data[MAX_VBO];
		GLfloat m_id_data[MAX_VBO];
		GLfloat m_color_data[MAX_VBO * 2];

		GLuint m_framebuffer;
		GLuint m_frametexture;
		GLuint m_framebuffer2;
		GLuint m_frametexture2;


		VertexArray *m_ScreenVAO;
		Buffer *m_ScreenVBO;
	
	public:

		//Initialize particle rendering
		PointRenderer(Window *window);

		//Add particle to render list
		void renderPoint(glm::vec2 pos, glm::vec4 color, int id);

		//Clears all particles
		void clear();

		//Render vertex array data to framebuffer
		void renderToFramebuffer(Shader *shader, int quadTexture, int framebufferindex);

		//Flush framebuffer to screen
		void flushFramebuffer(Shader *postshader, int framebufferindex);

		//Flush framebuffer to another framebuffer
		void flushFramebufferToFramebuffer(Shader *postshader, int first, int second);

		//Render vertex array data to screen
		void flush(Shader *shader, int pointtexture);
	
	};

}