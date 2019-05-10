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
#define DATA_PER_VERTEX		(sizeof(VertexData) / sizeof(GLfloat))
#define MAX_VBO				(MAX_POINTS_PER_FLUSH * VERTEX_PER_POINT * DATA_PER_VERTEX)

namespace oe {

	PointRenderer::PointRenderer() {
		m_buffer_current = 0;
		pointcount = 0;
		m_buffer_mapped = false;

		m_vao = new VertexArray();
		m_vbo = new Buffer(0, MAX_VBO, DATA_PER_VERTEX, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_vao->addBuffer(m_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexData), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(VertexData), (void*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(VertexData), (void*)(6 * sizeof(GLfloat)));
	}

	void PointRenderer::submitVertex(VertexData data) {
		if (!m_buffer_mapped) beginRendering();

		m_buffer[m_buffer_current] = data;
		m_buffer_current++;

		pointcount++;
	}

	void PointRenderer::draw(int pointtexture, int textureType) {
		stopRendering();

		glEnable(GL_DEPTH_TEST);

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
		m_buffer = (VertexData*)m_vbo->mapBuffer();
		m_buffer_mapped = true;
	}

	void PointRenderer::stopRendering() {
		if (m_buffer_mapped) m_vbo->unmapBuffer();
		m_buffer_mapped = false;
	}

}