#include "triangleRenderer.h"

#include "textureManager.h"
#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "buffers/indexBuffer.h"
#include "../utility/logger.h"


#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <iostream>



#define MAX_QUADS_PER_FLUSH (60000)
#define VERTEX_PER_TRIANGLE	(3)
#define DATA_PER_VERTEX		(sizeof(VertexData) / sizeof(GLfloat))
#define MAX_VBO				(MAX_QUADS_PER_FLUSH * VERTEX_PER_TRIANGLE * DATA_PER_VERTEX)

namespace oe {

	TriangleRenderer::TriangleRenderer(Window *window) {
		triangle_count = 0;
		m_buffer_current = 0;
		m_submit_triangle_vertex_index = 0;
		m_buffer_mapped = false;
		m_window = window;

		if (!m_window) return;

		m_VAO = new VertexArray();
		m_VBO = new Buffer(0, MAX_VBO, DATA_PER_VERTEX, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexData), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(VertexData), (void*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(VertexData), (void*)(6 * sizeof(GLfloat)));
	}

	void TriangleRenderer::beginRendering() {
		m_buffer = (VertexData*)m_VBO->mapBuffer();
		m_buffer_mapped = true;
	}

	void TriangleRenderer::stopRendering() {
		if (m_buffer_mapped) m_VBO->unmapBuffer();
		m_buffer_mapped = false;
	}

	void TriangleRenderer::submitVertex(VertexData data) {
		if (!m_buffer_mapped) beginRendering();

		m_buffer[m_buffer_current] = data;

		m_buffer_current++;
		m_submit_triangle_vertex_index++;

		if (m_submit_triangle_vertex_index > 2) {
			m_submit_triangle_vertex_index = 0;
			triangle_count++;
		}
	}

	void TriangleRenderer::draw(int texture, int textureType) {
		stopRendering();

		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, texture);
		m_VAO->bind();

		if (triangle_count > 0) {
			glDrawArrays(GL_TRIANGLES, 0, triangle_count * 3);
			triangle_count = 0;
			m_buffer_current = 0;
			m_submit_triangle_vertex_index = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}
}