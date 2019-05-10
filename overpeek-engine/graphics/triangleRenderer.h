﻿#pragma once

#include <glm/glm.hpp>

#include "vertexData.h"

namespace oe {

	class VertexArray;
	class Buffer;
	class IndexBuffer;

	class TriangleRenderer {
	private:
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
		TriangleRenderer();

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit all vertices for triangle
		//Returns index of that vertex
		void submitVertex(VertexData data);
		
		//Draws buffers and then clears them
		void draw(int texture, int textureType);

		//Submit sphere with radius r
		//x,y is the middle of the sphere
		//resolution is triangle count
		void submitSphere(float x, float y, float r, unsigned long long resolution, glm::vec4 color, int texture = 0);

	};
	
}