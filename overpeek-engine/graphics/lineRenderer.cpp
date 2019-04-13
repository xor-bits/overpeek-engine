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
#define DATA_PER_VERTEX		(sizeof(LineVertexData) / sizeof(GLfloat))
#define MAX_VBO				(MAX_LINES_PER_FLUSH * VERTEX_PER_LINE * DATA_PER_VERTEX)

namespace oe {

	LineRenderer::LineRenderer(Window *window) {
		oe::Logger::out(oe::info, "Creating renderer");
		m_indexcount = 0;
		m_buffer_current = 0;
		m_window = window;

		if (!m_window) return;

		m_VAO = new VertexArray();
		m_VBO = new Buffer(0, MAX_VBO, DATA_PER_VERTEX, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(LineVertexData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(LineVertexData), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(LineVertexData), (void*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(LineVertexData), (void*)(6 * sizeof(GLfloat)));
	}

	void LineRenderer::beginRendering() {
		m_buffer = (LineVertexData*)m_VBO->mapBuffer();
	}

	void LineRenderer::stopRendering() {
		m_VBO->unmapBuffer();
	}

	void LineRenderer::renderBox(glm::vec2 _pos1, glm::vec2 _pos2, int _id, glm::vec4 _color) {
		m_buffer[m_buffer_current].position.x = _pos1.x;
		m_buffer[m_buffer_current].position.y = _pos1.y;
		m_buffer[m_buffer_current].uv.x = 0.0;
		m_buffer[m_buffer_current].uv.y = 0.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_buffer[m_buffer_current].position.x = _pos2.x;
		m_buffer[m_buffer_current].position.y = _pos2.y;
		m_buffer[m_buffer_current].uv.x = 1.0;
		m_buffer[m_buffer_current].uv.y = 1.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_indexcount += VERTEX_PER_LINE;
	}

	void LineRenderer::draw(int texture, int textureType) {
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