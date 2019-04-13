#pragma once

#include <glm/glm.hpp>

#include "vertexData.h"

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class LineRenderer {
	private:
		VertexArray *m_VAO;
		Buffer *m_VBO;
		unsigned int m_indexcount;

		Window *m_window;

		VertexData* m_buffer;
		unsigned int m_buffer_current;
		unsigned int m_submit_quad_vertex_index;

		bool m_buffer_mapped;
		//int debug = 0;

	public:
		
		//Constructor
		LineRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit quad to renderer
		void submitVertex(VertexData data);
		
		//Draws buffers and then clears them
		void draw(int texture, int textureType);
	};
	
}