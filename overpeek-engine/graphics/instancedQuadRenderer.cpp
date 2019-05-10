#include "InstancedQuadRenderer.h"

#include "textureManager.h"
#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "buffers/indexBuffer.h"
#include "../utility/logger.h"


#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <iostream>


namespace oe {

	InstancedQuadRenderer::InstancedQuadRenderer(unsigned int max_instance_count)
	{

		GLushort indices[] = {
			0, 1, 2, 0, 2, 3
		};

		m_VAO = new VertexArray();
		m_VBO = new Buffer(0, max_instance_count, 3, sizeof(float), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(InstanceData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(InstanceData), (void*)(2 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(0, 1);
	}

	InstancedQuadRenderer::~InstancedQuadRenderer()
	{
	}

	void InstancedQuadRenderer::beginRendering() {
		m_buffer = (InstanceData*)m_VBO->mapBuffer();
		m_buffer_mapped = true;
	}

	void InstancedQuadRenderer::stopRendering() {
		if (m_buffer_mapped) m_VBO->unmapBuffer();
		m_buffer_mapped = false;
	}

	void InstancedQuadRenderer::submitInstance(InstanceData data) {
		if (!m_buffer_mapped) beginRendering();

		m_buffer[m_buffer_current++] = data;
	}

	void InstancedQuadRenderer::draw(int texture, int textureType) {
		stopRendering();

		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureType, texture);
		m_VAO->bind();

		if (m_buffer_current > 0) {
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_buffer_current);
			m_buffer_current = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}

}
