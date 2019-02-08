#pragma once

#include "shader.h"
#include "buffers/indexBuffer.h"
#include "buffers/buffer.h"
#include "buffers/vertexArray.h"
#include "../tools/logger.h"
#include "window.h"


///TODO: make use of geometry shader program

namespace graphics {

	class PointRenderer {
	private:
		Window *m_window;

		VertexArray *m_vao;
		Buffer *m_vbo;

		GLuint pointcount;
	
	public:

		//Initialize particle rendering
		PointRenderer(Window *window);

		//Add particle to render list
		void renderPoint(glm::vec2 pos, glm::vec4 color, int id);

		//Render vertex array data to screen
		void flush(Shader *shader, int pointtexture);
	
	};

}