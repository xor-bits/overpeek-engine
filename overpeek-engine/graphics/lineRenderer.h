#pragma once

#include <glm/glm.hpp>

namespace oe {

	class Window;
	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class LineRenderer {
	private:
		struct LineVertexData
		{
			glm::vec2 position;
			glm::vec2 uv;
			float texture;
			glm::vec4 color;
		};

		VertexArray *m_VAO;
		Buffer *m_VBO;
		unsigned int m_indexcount;

		Window *m_window;

		LineVertexData* m_buffer;
		unsigned int m_buffer_current;

		//int debug = 0;

	public:
		
		//Constructor
		LineRenderer(Window *window);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit quad to renderer
		void renderBox(glm::vec2 _pos1, glm::vec2 _pos2, int _id, glm::vec4 _color);
		
		//Draws buffers and then clears them
		void draw(int texture, int textureType);
	};
	
}