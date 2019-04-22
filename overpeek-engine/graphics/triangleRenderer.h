#pragma once

#include <glm/glm.hpp>

#include "vertexData.h"

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class TriangleRenderer {
	private:

		Window *m_window;

		VertexArray *m_VAO;
		Buffer *m_VBO;
		unsigned int triangle_count;

		VertexData* m_buffer;
		unsigned int m_buffer_current;
		unsigned int m_submit_triangle_vertex_index;

		bool m_buffer_mapped;
		//int debug = 0;

	public:
		
		//Constructor
		TriangleRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit all vertices for triangle
		//Returns index of that vertex
		void submitVertex(VertexData data);
		
		//Draws buffers and then clears them
		void draw(int texture, int textureType);
	};
	
}