#pragma once

#include <glm/glm.hpp>

#include "vertexData.h"

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class QuadRenderer {
	private:

		Window *m_window;

		VertexArray *m_VAO;
		Buffer *m_VBO;
		IndexBuffer *m_IBO;
		unsigned int m_indexcount;

		VertexData* m_buffer;
		unsigned int m_buffer_current;
		unsigned int m_submit_quad_vertex_index;

		bool m_buffer_mapped;
		//int debug = 0;

	public:
		
		//Constructor
		QuadRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit all vertices for quad
		//Returns index of that vertex
		void submitVertex(VertexData data);

		//void renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color);
		
		//Draws buffers and then clears them
		void draw(int texture, int textureType);
	};
	
}