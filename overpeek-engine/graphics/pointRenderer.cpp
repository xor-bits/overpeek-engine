#include "pointRenderer.h"

#include "textureManager.h"

#define MAX_POINTS_PER_FLUSH (60000)
#define VERTEX_PER_POINT	(1)
#define DATA_PER_VERTEX		(sizeof(TriangleVertexData) / sizeof(GLfloat))
#define INDEX_PER_QUAD		(6)
#define MAX_VBO				(MAX_POINTS_PER_FLUSH * VERTEX_PER_POINT * DATA_PER_VERTEX)
#define MAX_IBO				(MAX_POINTS_PER_FLUSH * INDEX_PER_QUAD)

namespace graphics {

	PointRenderer::PointRenderer(Window *window)
		: m_window(window)
	{
		pointcount = 0;
		m_window = window;

		if (!m_window) return;

		m_vao = new VertexArray();
		m_vbo = new Buffer(0, MAX_POINTS_PER_FLUSH * 2, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_id = new Buffer(0, MAX_POINTS_PER_FLUSH * 2, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_color = new Buffer(0, MAX_POINTS_PER_FLUSH * 4, 4, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_vao->addBuffer(m_vbo, 0);
		m_vao->addBuffer(m_id, 2);
		m_vao->addBuffer(m_color, 3);

		//Post processing
		return;
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

	void PointRenderer::renderPoint(glm::vec2 pos, glm::vec4 color, int id) {
		if (pointcount >= MAX_POINTS_PER_FLUSH) return;

		m_vertex_data[(pointcount * 2) + 0] = pos.x;
		m_vertex_data[(pointcount * 2) + 1] = pos.y;

		m_id_data[(pointcount * 2) + 0] = id;
		m_id_data[(pointcount * 2) + 1] = id;

		m_color_data[(pointcount * 2 * 2) + 0] = color.r;
		m_color_data[(pointcount * 2 * 2) + 1] = color.g;
		m_color_data[(pointcount * 2 * 2) + 2] = color.b;
		m_color_data[(pointcount * 2 * 2) + 3] = color.a;

		pointcount++;
	}

	void PointRenderer::flush(Shader *shader, int pointtexture) {
		glEnable(GL_DEPTH_TEST);

		if (!m_window) return;

		//Flush quads
		shader->enable();
		shader->setUniform1i("unif_text", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, graphics::TextureManager::getTexture(pointtexture));
		m_vao->bind();

		m_vbo->bind();
		m_vbo->setBufferData(m_vertex_data, pointcount * 2, 2, sizeof(GLfloat));
		m_id->bind();
		m_id->setBufferData(m_id, pointcount * 2, 2, sizeof(GLfloat));
		m_color->bind();
		m_color->setBufferData(m_color, pointcount * 4, 4, sizeof(GLfloat));

		//tools::Logger::info(pointcount);
		glDrawArrays(GL_POINTS, 0, pointcount);
		//glDrawElements(GL_POINTS, pointcount, GL_UNSIGNED_SHORT, 0);
		glDisable(GL_DEPTH_TEST);
	}

	void PointRenderer::clear() {
		pointcount = 0;
	}

}