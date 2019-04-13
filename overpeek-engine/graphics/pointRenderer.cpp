#include "pointRenderer.h"

#include "textureManager.h"
#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "../utility/logger.h"


#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <iostream>


#define MAX_POINTS_PER_FLUSH (60000)
#define VERTEX_PER_POINT	(1)
#define DATA_PER_VERTEX		(sizeof(PointVertexData) / sizeof(GLfloat))
#define MAX_VBO				(MAX_POINTS_PER_FLUSH * VERTEX_PER_POINT * DATA_PER_VERTEX)

namespace oe {

	PointRenderer::PointRenderer(Window *window)
		: m_window(window)
	{
		m_buffer_current = 0;
		pointcount = 0;
		m_window = window;

		if (!m_window) return;

		m_vao = new VertexArray();
		m_vbo = new Buffer(0, MAX_VBO, DATA_PER_VERTEX, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_vao->addBuffer(m_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PointVertexData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(PointVertexData), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(PointVertexData), (void*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(PointVertexData), (void*)(6 * sizeof(GLfloat)));
	}

	void PointRenderer::renderPoint(glm::vec2 pos, glm::vec2 size, int id, glm::vec4 color) {

		//if (pointcount > MAX_POINTS_PER_FLUSH) return;

		m_buffer[m_buffer_current].position = pos;
		m_buffer[m_buffer_current].size = size;
		m_buffer[m_buffer_current].texture = id;
		m_buffer[m_buffer_current].color = color;
		m_buffer_current++;

		pointcount++;
	}

	void PointRenderer::draw(int pointtexture, int textureType) {
		glEnable(GL_DEPTH_TEST);

		if (!m_window) return;

		//Flush quads
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, pointtexture);
		m_vao->bind();

		//Actual drawing
		if (pointcount > 0) {
			glDrawArrays(GL_POINTS, 0, pointcount);
			m_buffer_current = 0;
			pointcount = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}

	void PointRenderer::beginRendering() {
		m_buffer = (PointVertexData*)m_vbo->mapBuffer();
	}

	void PointRenderer::stopRendering() {
		m_vbo->unmapBuffer();
	}

}