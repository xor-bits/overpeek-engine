#pragma once

#include <glm/glm.hpp>

#include "vertexData.h"

///TODO: make use of geometry shader program

namespace oe {

	class VertexArray;
	class Buffer;
	class IndexBuffer;
	class Window;

	class PointRenderer {
	private:

		VertexArray *m_vao;
		Buffer *m_vbo;
		int pointcount;

		Window *m_window;

		VertexData* m_buffer;
		int m_buffer_current;

		bool m_buffer_mapped;

	public:

		//Initialize particle rendering
		PointRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit point to renderer
		void submitVertex(VertexData data);

		//Draws buffers and then clears them
		void draw(int pointtexture, int textureType);
	
	};

}