#include "quadRenderer.h"

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
#define VERTEX_PER_QUAD		(4)
#define DATA_PER_VERTEX		(sizeof(VertexData) / sizeof(GLfloat))
#define INDEX_PER_QUAD		(6)
#define MAX_VBO				(MAX_QUADS_PER_FLUSH * VERTEX_PER_QUAD * DATA_PER_VERTEX)
#define MAX_IBO				(MAX_QUADS_PER_FLUSH * INDEX_PER_QUAD)

namespace oe {

	QuadRenderer::QuadRenderer(Window *window) {
		m_indexcount = 0;
		m_buffer_current = 0;
		m_submit_quad_vertex_index = 0;
		m_buffer_mapped = false;
		m_window = window;

		if (!m_window) return;

		GLushort *indices = new GLushort[MAX_IBO];
		int offset = 0;
		for (int i = 0; i < MAX_QUADS_PER_FLUSH; i++)
		{
			indices[i * 6 + 0] = offset + 0;
			indices[i * 6 + 1] = offset + 1;
			indices[i * 6 + 2] = offset + 2;
			indices[i * 6 + 3] = offset + 0;
			indices[i * 6 + 4] = offset + 2;
			indices[i * 6 + 5] = offset + 3;
			offset += 4;
		}

		m_VAO = new VertexArray();
		m_IBO = new IndexBuffer(indices, MAX_IBO, GL_STATIC_DRAW);
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

	void QuadRenderer::beginRendering() {
		m_buffer = (VertexData*)m_VBO->mapBuffer();
		m_buffer_mapped = true;
	}

	void QuadRenderer::stopRendering() {
		if (m_buffer_mapped) m_VBO->unmapBuffer();
		m_buffer_mapped = false;
	}

	void QuadRenderer::submitVertex(VertexData data) {
		if (!m_buffer_mapped) beginRendering();

		m_buffer[m_buffer_current] = data;

		m_buffer_current++;
		m_submit_quad_vertex_index++;

		if (m_submit_quad_vertex_index > 3) {
			m_submit_quad_vertex_index = 0;
			m_indexcount += INDEX_PER_QUAD;
		}
	}

	void QuadRenderer::draw(int texture, int textureType) {
		stopRendering();

		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, texture);
		m_VAO->bind();

		//oe::Logger::out("Quads: ", m_indexcount / 6);
		if (m_indexcount > 0) {
			glDrawElements(GL_TRIANGLES, m_indexcount, GL_UNSIGNED_SHORT, 0);
			m_indexcount = 0;
			m_buffer_current = 0;
			m_submit_quad_vertex_index = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}
}