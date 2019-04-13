#include "lineRenderer.h"

#include "textureManager.h"
#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "buffers/indexBuffer.h"
#include "../utility/logger.h"


#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <iostream>



#define MAX_LINES_PER_FLUSH (60000)
#define VERTEX_PER_LINE		(2)
#define DATA_PER_VERTEX		(sizeof(VertexData) / sizeof(GLfloat))
#define MAX_VBO				(MAX_LINES_PER_FLUSH * VERTEX_PER_LINE * DATA_PER_VERTEX)

namespace oe {

	LineRenderer::LineRenderer(Window *window) {
		m_indexcount = 0;
		m_buffer_current = 0;
		m_submit_quad_vertex_index = 0;
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

	void LineRenderer::beginRendering() {
		m_buffer = (VertexData*)m_VBO->mapBuffer();
		m_buffer_mapped = true;
	}

	void LineRenderer::stopRendering() {
		if (m_buffer_mapped) m_VBO->unmapBuffer();
		m_buffer_mapped = false;
	}

	void LineRenderer::submitVertex(VertexData data) {
		if (!m_buffer_mapped) beginRendering();

		m_buffer[m_buffer_current] = data;

		m_buffer_current++;
		m_submit_quad_vertex_index++;

		if (m_submit_quad_vertex_index > 1) {
			m_submit_quad_vertex_index = 0;
			m_indexcount += VERTEX_PER_LINE;
		}
	}

	void LineRenderer::draw(int texture, int textureType) {
		stopRendering();

		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, texture);
		m_VAO->bind();

		if (m_indexcount > 0) {
			glDrawArrays(GL_LINES, 0, m_indexcount);
			m_indexcount = 0;
			m_buffer_current = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}
}