#include "gl_framebuffer.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace oe::graphics {

	GLFrameBuffer::GLFrameBuffer(const FrameBufferInfo& framebuffer_info, Window* window)
		: FrameBuffer(framebuffer_info, window) 
	{
		oe_debug_call("gl_framebuffer");

		glGenFramebuffers(1, &m_id);
		bind();

		// Texture
		TextureInfo texture_info = {};
		texture_info.empty = true;
		texture_info.size = { framebuffer_info.width, framebuffer_info.height };
		texture_info.offset = { 0, 0 };
		m_texture = new oe::graphics::GLTexture(texture_info);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->getGLTexture(), 0);

		// Render buffer object
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_info.width, framebuffer_info.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		// FBO
		int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			oe_error_terminate("Framebuffer not complete {}", fboStatus);

		clear();
		window->bind();
	}

	GLFrameBuffer::~GLFrameBuffer() {
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);
		delete m_texture;
	}

	void GLFrameBuffer::clear(const glm::vec4& color) {
		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glViewport(0, 0, m_framebuffer_info.width, m_framebuffer_info.height);
	}

}
