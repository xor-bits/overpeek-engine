#include "renderer.h"


namespace graphics {

	Renderer::Renderer(std::string fontpath) {
		quadCount = 0;

		for (int i = 0; i < MAX_QUADS_PER_FLUSH; i++)
		{
			m_index[(i * INDEX_PER_QUAD) + 0] = (i * 4) + 0;
			m_index[(i * INDEX_PER_QUAD) + 1] = (i * 4) + 1;
			m_index[(i * INDEX_PER_QUAD) + 2] = (i * 4) + 2;
			m_index[(i * INDEX_PER_QUAD) + 3] = (i * 4) + 0;
			m_index[(i * INDEX_PER_QUAD) + 4] = (i * 4) + 2;
			m_index[(i * INDEX_PER_QUAD) + 5] = (i * 4) + 3;
		}

		m_VAO = new VertexArray();
		m_IBO = new IndexBuffer(m_index, MAX_IBO, GL_STATIC_DRAW);
		m_VBO = new Buffer(0, MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_UV = new Buffer(0, MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO, 0);
		m_VAO->addBuffer(m_UV, 1);

		m_VAO->bind();
		m_VAO->unbind();
	}

	void Renderer::initText(std::string fontPath) {
	}


	void Renderer::renderBox(float x, float y, float w, float h, float t_x, float t_y, float t_w, float t_h) {
		//0
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 0] = x + 0; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 1] = y + 0;
		//1
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 2] = x + 0; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 3] = y + h;
		//2
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 4] = x + w; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 5] = y + h;
		//3
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 6] = x + w; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 7] = y + 0;

		//0
		m_uv[(quadCount * VERTEX_PER_QUAD) + 0] = t_x + 0;
		m_uv[(quadCount * VERTEX_PER_QUAD) + 1] = t_y + 0;
		//1
		m_uv[(quadCount * VERTEX_PER_QUAD) + 2] = t_x + 0;
		m_uv[(quadCount * VERTEX_PER_QUAD) + 3] = t_y + t_h;
		//2
		m_uv[(quadCount * VERTEX_PER_QUAD) + 4] = t_x + t_w;
		m_uv[(quadCount * VERTEX_PER_QUAD) + 5] = t_y + t_h;
		//3
		m_uv[(quadCount * VERTEX_PER_QUAD) + 6] = t_x + t_w;
		m_uv[(quadCount * VERTEX_PER_QUAD) + 7] = t_y + 0;

		quadCount++;
	}

	void Renderer::renderText(float x, float y, float w, float h, std::string text, glm::vec3 color, int xAlign, int yAlign) {
		//m_fontLoader->renderText(m_textureshader, text, x, y, w, h, color, xAlign, yAlign);
	}

	void Renderer::flush() {
		m_VAO->bind();

		//m_VBO->bind();
		//GLfloat *buffer = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		//buffer = m_vertex;
		//glUnmapBuffer(GL_ARRAY_BUFFER);
		m_VBO->setBufferData(m_vertex, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_UV->setBufferData(m_uv, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));

		glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_SHORT, 0);
	}

	void Renderer::clear() {
		quadCount = 0;
	}
}