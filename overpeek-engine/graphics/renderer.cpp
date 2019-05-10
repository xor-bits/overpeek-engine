#include "renderer.h"

#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "window.h"
#include "../utility/logger.h"
#include "shader.h"

#include <GL/glew.h>

namespace oe {

	Renderer::Renderer(const char *font, int window_width, int window_height) {
		quadRenderer = new QuadRenderer();
		fontRenderer = new FontRenderer(font, this);
		pointRenderer = new PointRenderer();
		lineRenderer = new LineRenderer();
		triangleRenderer = new TriangleRenderer();
		shader = new Shader();

		//Post processing
		post_shader = new Shader();

		// The fullscreen quad's VBO
		GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f, -1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f
		};

		m_ScreenVAO = new VertexArray();
		m_ScreenVBO = new Buffer(g_quad_vertex_buffer_data, 12, 2, sizeof(GLfloat), GL_STATIC_DRAW);
		m_ScreenVAO->addBuffer(m_ScreenVBO, 0);
		m_ScreenVAO->unbind();

		//Render buffer object
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);

		//First framebuffer and frametexture
		//buffer
		glGenFramebuffers(1, &m_framebuffer1);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);

		//texture
		glGenTextures(1, &m_frametexture1);
		glBindTexture(GL_TEXTURE_2D, m_frametexture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture1, 0);

		//attach
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) oe::Logger::out("Framebuffer is not complete!", oe::error);

		//Second framebuffer and frametexture
		//buffer
		glGenFramebuffers(1, &m_framebuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

		//texture
		glGenTextures(1, &m_frametexture2);
		glBindTexture(GL_TEXTURE_2D, m_frametexture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frametexture2, 0);

		//attach
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) oe::Logger::out("Framebuffer is not complete!", oe::error);

		//Unbinding
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Renderer::~Renderer() {
		delete fontRenderer;
		delete quadRenderer;
		delete pointRenderer;
	}

	//Draws all quads and text
	//textbool is location of int (used as boolean) in shader that enables or disables text rendering
	void Renderer::draw(int texture, bool textureArray) {

		int textureType = GL_TEXTURE_2D;
		if (textureArray) textureType = GL_TEXTURE_2D_ARRAY;

		shader->enable();
		pointRenderer->draw(texture, textureType);
		quadRenderer->draw(texture, textureType);
		lineRenderer->draw(texture, textureType);
		triangleRenderer->draw(texture, textureType);

		//Last is text
		fontRenderer->draw();
	}

	//Draws all quads and text to specified framebuffer at index
	void Renderer::drawToFramebuffer(int texture, bool textureArray, bool first_framebuffer, int window_width, int window_height) {
		if (first_framebuffer) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
		else glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int textureType = GL_TEXTURE_2D;
		if (textureArray) textureType = GL_TEXTURE_2D_ARRAY;
		draw(texture, textureArray);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window_width, window_height);
	}

	//Draws (first or second) framebuffer to screen
	void Renderer::drawFramebuffer(const char *texture_sampler, bool first_framebuffer) {
		//Render quad to whole screen
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		post_shader->enable();
		post_shader->setUniform1i(texture_sampler, 0);
		glActiveTexture(GL_TEXTURE0);

		if (first_framebuffer) glBindTexture(GL_TEXTURE_2D, m_frametexture1);
		else glBindTexture(GL_TEXTURE_2D, m_frametexture2);


		m_ScreenVAO->bind();
		m_ScreenVBO->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
	}

	//Draws framebuffer to another framebuffer
	void Renderer::drawFramebufferToFramebuffer(const char *texture_sampler, bool first_framebuffer) {
		if (first_framebuffer) glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
		else glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);

		drawFramebuffer(texture_sampler, !first_framebuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}