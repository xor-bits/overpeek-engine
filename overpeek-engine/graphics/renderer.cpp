#include "renderer.h"
#include "textureManager.h"


namespace graphics {

	Renderer::Renderer(std::string fontpath) : Renderer::Renderer() {
		textRender = true;
		initText(fontpath);
	}

	Renderer::Renderer() {
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
		m_COLOR = new Buffer(0, MAX_VBO * 2, 4, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO, 0);
		m_VAO->addBuffer(m_UV, 1);
		m_VAO->addBuffer(m_ID, 2);
		m_VAO->addBuffer(m_COLOR, 3);

		//Post processing

		// The fullscreen quad's VBO
		GLfloat g_quad_vertex_buffer_data[] = {
			-0.2f, -0.2f, 0.0f,
			 0.2f, -0.2f, 0.0f,
			-0.2f,  0.2f, 0.0f,
			-0.2f,  0.2f, 0.0f,
			 0.2f, -0.2f, 0.0f,
			 0.2f,  0.2f, 0.0f,
		};
		
		m_quad_vertexbuffer;
		glGenBuffers(1, &m_quad_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_quad_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
		
		
		
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		
		m_frametexture;
		glGenTextures(1, &m_frametexture);
		
		glBindTexture(GL_TEXTURE_2D, m_frametexture);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture, 0);
		
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
		
		while (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer::initText(std::string fontPath) {
		fontLoader = new FontLoader(fontPath);
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

	/*Angle is radian*/
	void Renderer::renderBox(float x, float y, float w, float h, float angle, int textureID, glm::vec4 color) {
		///TODO FIX:
		x = x / 2.0;
		y = y / 2.0;
		

		glm::vec2 pnt = rotatePoint(x, y, angle, glm::vec2(x, y));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 0] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 1] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x, y + h));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 2] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 3] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x + w, y + h));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 4] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 5] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x + w, y));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 6] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 7] = y + pnt.y;

		m_id[(quadCount * VERTEX_PER_QUAD) + 0] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 1] = textureID;

		m_id[(quadCount * VERTEX_PER_QUAD) + 2] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 3] = textureID;

		m_id[(quadCount * VERTEX_PER_QUAD) + 4] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 5] = textureID;

		m_id[(quadCount * VERTEX_PER_QUAD) + 6] = textureID;
		m_id[(quadCount * VERTEX_PER_QUAD) + 7] = textureID;

		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 0] = color.r;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 1] = color.g;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 2] = color.b;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 3] = color.a;

		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 4] = color.r;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 5] = color.g;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 6] = color.b;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 7] = color.a;

		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 8] = color.r;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 9] = color.g;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 10] = color.b;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 11] = color.a;

		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 12] = color.r;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 13] = color.g;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 14] = color.b;
		m_color[(quadCount * VERTEX_PER_QUAD * 2) + 15] = color.a;

		quadCount++;
	}

	/*Angle is radian*/
	void Renderer::renderBoxCentered(float x, float y, float w, float h, float angle, int textureID, glm::vec4 color) {
		glm::vec2 pnt = rotatePoint(x, y, angle, glm::vec2(x - w / 2.0, y - h / 2.0));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 0] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 1] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x - w / 2.0, y + h / 2.0));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 2] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 3] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x + w / 2.0, y + h / 2.0));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 4] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 5] = y + pnt.y;
		pnt = rotatePoint(x, y, angle, glm::vec2(x + w / 2.0, y - h / 2.0));
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 6] = x + pnt.x;
		m_vertex[(quadCount * VERTEX_PER_QUAD) + 7] = y + pnt.y;

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
		glActiveTexture(GL_TEXTURE0);
		//Flush quads
		shader->enable();
		shader->setUniform1i("unif_text", 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, graphics::TextureManager::getTexture(quadTexture));
		m_VAO->bind();
		
		m_VBO->setBufferData(m_vertex, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_ID->setBufferData(m_id, quadCount * VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_COLOR->setBufferData(m_color, quadCount * VERTEX_PER_QUAD * 2, 4, sizeof(GLfloat));
		
		glDrawElements(GL_TRIANGLES, quadCount * 6, GL_UNSIGNED_SHORT, 0);
	
		//FLush text
		shader->setUniform1i("unif_text", 1);

		if (textRender) fontLoader->flush();
	}

	GLuint Renderer::renderToFramebuffer(Shader *postshader, Shader *shader, int quadTexture) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glViewport(0, 0, 1280, 720); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		flush(shader, quadTexture);

		postshader->enable();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720); // Render on the whole framebuffer, complete from the lower left corner to the upper right


		// Clear the screen
		glClearColor(0.5, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_frametexture);
		postshader->setUniform1i("renderedTexture", 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_quad_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

		return m_framebuffer;
	}

	void Renderer::clear() {
		quadCount = 0;
	}

	/*Angle is radian*/
	void Renderer::renderText(float x, float y, float w, float h, float angle, std::string text, glm::vec4 color, int xAlign, int yAlign) {// Activate corresponding render state
		if (textRender) fontLoader->renderText(x, y, w, h, text, color, xAlign, yAlign);
	}
}