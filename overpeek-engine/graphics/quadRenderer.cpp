#include "quadRenderer.h"
#include "textureManager.h"


#define MAX_QUADS_PER_FLUSH (60000)
#define VERTEX_PER_QUAD		(4)
#define DATA_PER_VERTEX		(sizeof(TriangleVertexData) / sizeof(GLfloat))
#define INDEX_PER_QUAD		(6)
#define MAX_VBO				(MAX_QUADS_PER_FLUSH * VERTEX_PER_QUAD * DATA_PER_VERTEX)
#define MAX_IBO				(MAX_QUADS_PER_FLUSH * INDEX_PER_QUAD)

namespace graphics {

	QuadRenderer::QuadRenderer(Window *window) {
		tools::Logger::info("Creating renderer");
		m_indexcount = 0;
		m_buffer_current = 0;
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
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(TriangleVertexData), (void*)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(TriangleVertexData), (void*)(2 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(TriangleVertexData), (void*)(4 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(TriangleVertexData), (void*)(5 * sizeof(GLfloat)));
	}

	glm::vec2 rotatePoint(float cx, float cy, float angle, glm::vec2 p)
	{
		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		p.x -= cx;
		p.y -= cy;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + cx;
		p.y = ynew + cy;
		return p;
	}

	void QuadRenderer::beginRendering() {
		m_buffer = (TriangleVertexData*)m_VBO->mapBuffer();
	}

	void QuadRenderer::stopRendering() {
		m_VBO->unmapBuffer();
	}

	void QuadRenderer::renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color) {
		m_buffer[m_buffer_current].position.x = _pos.x;
		m_buffer[m_buffer_current].position.y = _pos.y;
		m_buffer[m_buffer_current].uv.x = 0.0;
		m_buffer[m_buffer_current].uv.y = 0.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_buffer[m_buffer_current].position.x = _pos.x;
		m_buffer[m_buffer_current].position.y = _pos.y + _size.y;
		m_buffer[m_buffer_current].uv.x = 0.0;
		m_buffer[m_buffer_current].uv.y = 1.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_buffer[m_buffer_current].position.x = _pos.x + _size.x;
		m_buffer[m_buffer_current].position.y = _pos.y + _size.y;
		m_buffer[m_buffer_current].uv.x = 1.0;
		m_buffer[m_buffer_current].uv.y = 1.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_buffer[m_buffer_current].position.x = _pos.x + _size.x;
		m_buffer[m_buffer_current].position.y = _pos.y;
		m_buffer[m_buffer_current].uv.x = 1.0;
		m_buffer[m_buffer_current].uv.y = 0.0;
		m_buffer[m_buffer_current].texture = _id;
		m_buffer[m_buffer_current].color = _color;
		m_buffer_current++;

		m_indexcount += INDEX_PER_QUAD;
	}

	void QuadRenderer::draw(GLint texture) {
		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		//Actual rendering
		//tools::Logger::info(debug);
		//debug = 0;
		if (m_indexcount > 0) {
			m_VAO->bind();
			glDrawElements(GL_TRIANGLES, m_indexcount, GL_UNSIGNED_SHORT, 0);
			m_indexcount = 0;
			m_buffer_current = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}
}