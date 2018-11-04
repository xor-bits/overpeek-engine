#include "renderer.h"
#include "textureManager.h"


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

			m_uv[(i * VERTEX_PER_QUAD) + 0] = 0;
			m_uv[(i * VERTEX_PER_QUAD) + 1] = 0;
			m_uv[(i * VERTEX_PER_QUAD) + 2] = 0;
			m_uv[(i * VERTEX_PER_QUAD) + 3] = 1;
			m_uv[(i * VERTEX_PER_QUAD) + 4] = 1;
			m_uv[(i * VERTEX_PER_QUAD) + 5] = 1;
			m_uv[(i * VERTEX_PER_QUAD) + 6] = 1;
			m_uv[(i * VERTEX_PER_QUAD) + 7] = 0;
		}

		m_VAO = new VertexArray();
		m_IBO = new IndexBuffer(m_index, MAX_IBO, GL_STATIC_DRAW);
		m_UV = new Buffer(m_uv, MAX_VBO, 2, sizeof(GLfloat), GL_STATIC_DRAW);
		m_VBO = new Buffer(0, MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_ID = new Buffer(0, MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO, 0);
		m_VAO->addBuffer(m_UV, 1);
		m_VAO->addBuffer(m_ID, 2);

		initText(fontpath);
	}

	void Renderer::initText(std::string fontPath) {
		fontLoader = new FontLoader(fontPath);
	}

	void Renderer::renderBox(float x, float y, float w, float h, int textureID) {
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 0] = x; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 1] = y;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 2] = x; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 3] = y + h;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 4] = x + w; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 5] = y + h;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 6] = x + w; 
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 7] = y;

		m_id[(quadCount * VERTEX_PER_QUAD) + 0] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 1] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 2] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 3] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 4] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 5] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 6] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 7] = textureID;

		quadCount++;
	}

	void Renderer::flush(Shader *shader, int quadTexture) {
		//Flush quads
		shader->enable();
		shader->setUniform1i("unif_text", 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, graphics::TextureManager::getTexture(quadTexture));
		m_VAO->bind();
		
		m_VBO->setBufferData(m_vertex, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_ID->setBufferData(m_id, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		
		glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_SHORT, 0);
	
		//FLush text
		shader->setUniform1i("unif_text", 1);
		fontLoader->flush();
	}

	void Renderer::clear() {
		quadCount = 0;
	}

	void Renderer::renderText(float x, float y, float w, float h, std::string text, glm::vec3 color, int xAlign, int yAlign) {// Activate corresponding render state
		fontLoader->renderText(x, y, w, h, text, color, xAlign, yAlign);
	}
}