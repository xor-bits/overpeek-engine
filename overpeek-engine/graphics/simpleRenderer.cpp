#include "simpleRenderer.h"


namespace graphics {

	VertexArray *SimpleRenderer::m_VAO;
	Buffer *SimpleRenderer::m_VBO;
	Buffer *SimpleRenderer::m_TBO;
	IndexBuffer *SimpleRenderer::m_IBO;

	FontLoader *SimpleRenderer::m_fontLoader;
	Shader *SimpleRenderer::m_shader;
	Shader *SimpleRenderer::m_textureshader;
	Shader *SimpleRenderer::m_textShader;

	glm::vec4 SimpleRenderer::m_color;

	bool renderInitialized;
	bool fontInitialized;

	void SimpleRenderer::init(Shader *shader, Shader *textureshader, Shader *textShader, std::string fontpath) {
		m_shader = shader;
		m_textureshader = textureshader;
		m_textShader = textShader;

		m_fontLoader = new FontLoader(fontpath);

		m_color = glm::vec4(1.0);

		GLfloat textureCoords[] = {
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0
		};

		GLushort indices[] = {
			0, 1, 2,
			0, 2, 3
		};

		m_VAO = new VertexArray();
		m_IBO = new IndexBuffer(indices, 6);
		m_VBO = new Buffer(0, 8, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_TBO = new Buffer(textureCoords, 8, 2, sizeof(GLfloat), GL_STATIC_DRAW);
		m_VAO->addBuffer(m_VBO, 0);
		m_VAO->addBuffer(m_TBO, 1);
	}

	void SimpleRenderer::renderBox(float x, float y, float w, float h) {
		glm::mat4 ml_matrix = glm::mat4(1.0f);
		m_shader->enable();
		m_shader->SetUniformMat4("ml_matrix", ml_matrix);
		m_shader->setUniform4f("color", m_color);
		m_VAO->bind();
		
		GLfloat vertices[] = {
			x + 0, y + 0,
			x + 0, y + h,
			x + w, y + h,
			x + w, y + 0
		};
		m_VBO->setBufferData(vertices, 8, 2, sizeof(GLfloat));
		
		m_IBO->bind();
		glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_SHORT, 0);
	}

	void SimpleRenderer::renderBox(float x, float y, float w, float h, int texture) {
		glm::mat4 ml_matrix = glm::mat4(1.0f);
		m_textureshader->enable();
		m_textureshader->SetUniformMat4("ml_matrix", ml_matrix);
		m_textureshader->setUniform4f("color", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		m_textureshader->setUniform1i("overwrite_off", texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		
		m_VAO->bind();
		
		GLfloat vertices[] = {
			x + 0, y + 0,
			x + 0, y + h,
			x + w, y + h,
			x + w, y + 0
		};
		m_VBO->setBufferData(vertices, 8, 2, sizeof(GLfloat));
		
		m_IBO->bind();
		glDrawElements(GL_TRIANGLES, m_IBO->getCount(), GL_UNSIGNED_SHORT, 0);
	}

	void SimpleRenderer::renderText(float x, float y, float w, float h, std::string text, glm::vec3 color, int xAlign, int yAlign) {
		m_fontLoader->renderText(m_textShader, text, x, y, w, h, color, xAlign, yAlign);
	}

}