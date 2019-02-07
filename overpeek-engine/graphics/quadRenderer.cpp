#include "quadRenderer.h"
#include "textureManager.h"


namespace graphics {

	QuadRenderer::QuadRenderer(Window *window) {
		tools::Logger::info("Creating renderer");
		m_indexcount = 0;
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
		//tools::Logger::info(0, DATA_PER_VERTEX);
		//tools::Logger::info(1, MAX_VBO);
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

		//Post processing

		// The fullscreen quad's VBO
		GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f,
		};
		
		m_ScreenVAO = new VertexArray();
		m_ScreenVBO = new Buffer(g_quad_vertex_buffer_data, 18, 2, sizeof(GLfloat), GL_STATIC_DRAW);
		m_ScreenVAO->addBuffer(m_ScreenVBO, 0);
		m_ScreenVAO->unbind();

		//Render buffer object
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_window->getWidth(), m_window->getHeight());

		//First framebuffer and frametexture
		//buffer
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		//texture
		glGenTextures(1, &m_frametexture);
		glBindTexture(GL_TEXTURE_2D, m_frametexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window->getWidth(), m_window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture, 0);

		//attach
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) tools::Logger::error("Framebuffer is not complete!");

		//Second framebuffer and frametexture
		//buffer
		glGenFramebuffers(1, &m_framebuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

		//texture
		glGenTextures(1, &m_frametexture2);
		glBindTexture(GL_TEXTURE_2D, m_frametexture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_window->getWidth(), m_window->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture2, 0);

		//attach
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) tools::Logger::error("Framebuffer is not complete!");

		//Unbinding
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		m_buffer->position.x = _pos.x;
		m_buffer->position.y = _pos.y;
		m_buffer->uv.x = 0.0;
		m_buffer->uv.y = 0.0;
		m_buffer->texture = _id;
		m_buffer->color = _color;
		m_buffer++;

		m_buffer->position.x = _pos.x;
		m_buffer->position.y = _pos.y + _size.y;
		m_buffer->uv.x = 0.0;
		m_buffer->uv.y = 1.0;
		m_buffer->texture = _id;
		m_buffer->color = _color;
		m_buffer++;

		m_buffer->position.x = _pos.x + _size.x;
		m_buffer->position.y = _pos.y + _size.y;
		m_buffer->uv.x = 1.0;
		m_buffer->uv.y = 1.0;
		m_buffer->texture = _id;
		m_buffer->color = _color;
		m_buffer++;

		m_buffer->position.x = _pos.x + _size.x;
		m_buffer->position.y = _pos.y;
		m_buffer->uv.x = 1.0;
		m_buffer->uv.y = 0.0;
		m_buffer->texture = _id;
		m_buffer->color = _color;
		m_buffer++;

		
		/////TODO FIX:
		//x = x / 2.0;
		//y = y / 2.0;
		//
		//
		//glm::vec2 pnt = rotatePoint(x, y, angle, glm::vec2(x, y));
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 0] = x + pnt.x;
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 1] = y + pnt.y;
		//pnt = rotatePoint(x, y, angle, glm::vec2(x, y + h));
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 2] = x + pnt.x;
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 3] = y + pnt.y;
		//pnt = rotatePoint(x, y, angle, glm::vec2(x + w, y + h));
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 4] = x + pnt.x;
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 5] = y + pnt.y;
		//pnt = rotatePoint(x, y, angle, glm::vec2(x + w, y));
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 6] = x + pnt.x;
		//m_vertex[(quadCount * VERTEX_PER_QUAD) + 7] = y + pnt.y;
		//
		//m_id[(quadCount * VERTEX_PER_QUAD) + 0] = textureID;
		//m_id[(quadCount * VERTEX_PER_QUAD) + 1] = textureID;
		//
		//m_id[(quadCount * VERTEX_PER_QUAD) + 2] = textureID;
		//m_id[(quadCount * VERTEX_PER_QUAD) + 3] = textureID;
		//
		//m_id[(quadCount * VERTEX_PER_QUAD) + 4] = textureID;
		//m_id[(quadCount * VERTEX_PER_QUAD) + 5] = textureID;
		//
		//m_id[(quadCount * VERTEX_PER_QUAD) + 6] = textureID;
		//m_id[(quadCount * VERTEX_PER_QUAD) + 7] = textureID;
		//
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 0] = color.r;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 1] = color.g;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 2] = color.b;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 3] = color.a;
		//
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 4] = color.r;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 5] = color.g;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 6] = color.b;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 7] = color.a;
		//
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 8] = color.r;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 9] = color.g;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 10] = color.b;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 11] = color.a;
		//
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 12] = color.r;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 13] = color.g;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 14] = color.b;
		//m_color[(quadCount * VERTEX_PER_QUAD * 2) + 15] = color.a;
		
		m_indexcount += INDEX_PER_QUAD;
	}

	void QuadRenderer::draw(GLint texture) {
		glEnable(GL_DEPTH_TEST);

		//Binding
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		//Actual rendering
		if (m_indexcount > 0) {
			tools::Logger::info(m_indexcount);
			m_VAO->bind();
			glDrawElements(GL_TRIANGLES, m_indexcount, GL_UNSIGNED_SHORT, 0);
			m_indexcount = 0;
		}

		glDisable(GL_DEPTH_TEST);
	}

	void QuadRenderer::renderToFramebuffer(Shader *shader, int quadTexture, int framebufferindex) {
		if (framebufferindex == 0) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		else if (framebufferindex == 1) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw(quadTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	}

	void QuadRenderer::drawFramebufferToFramebuffer(Shader *postshader, int first, int second) {
		if (second == 0) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		else if (second == 1) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
		
		drawFramebuffer(postshader, first);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void QuadRenderer::drawFramebuffer(Shader *postshader, int framebufferindex) {
		//Render quad to whole screen
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		postshader->enable();
		postshader->setUniform1i("renderedTexture", 0);
		glActiveTexture(GL_TEXTURE0);

		if (framebufferindex == 0) {
			glBindTexture(GL_TEXTURE_2D, m_frametexture);
		}
		else if (framebufferindex == 1) {
			glBindTexture(GL_TEXTURE_2D, m_frametexture2);
		}
		

		m_ScreenVAO->bind();
		m_ScreenVBO->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
	}
}