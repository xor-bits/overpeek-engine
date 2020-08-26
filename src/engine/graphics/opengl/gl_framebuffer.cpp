#include "gl_framebuffer.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>



namespace oe::graphics
{
	uint32_t GLFrameBuffer::bound_fbo_id = 0xFFFFFFFF;

	GLFrameBuffer::GLFrameBuffer(const FrameBufferInfo& framebuffer_info, const Window& window)
		: IFrameBuffer(framebuffer_info, window) 
	{
		oe_debug_call("gl_framebuffer");

		glGenFramebuffers(1, &m_id);
		bind();

		// Texture
		TextureInfo texture_info = {};
		texture_info.empty = true;
		texture_info.size = { static_cast<size_t>(framebuffer_info.size.x), static_cast<size_t>(framebuffer_info.size.y) };
		texture_info.offset = { 0, 0 };
		m_texture = Texture(texture_info);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reinterpret_cast<GLTexture*>(m_texture.get())->getGLTarget(), reinterpret_cast<GLTexture*>(m_texture.get())->getGLTexture(), 0);

		// Render buffer object
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer_info.size.x, framebuffer_info.size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		// FBO
		int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			oe_error_terminate("Framebuffer not complete {}", fboStatus);

		clear();
		window->bind();
		
		sprite.m_owner = m_texture;
		sprite.position = { 0.0f, 1.0f };
		sprite.size = { 1.0f, -1.0f };
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);
	}

	void GLFrameBuffer::clear(const glm::vec4& color)
	{
		bind();

		glClearColor(color.x, color.y, color.z, color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFrameBuffer::bind()
	{
		if (bound_fbo_id == m_id) return;
		bound_fbo_id = m_id;

		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		glViewport(0, 0, m_framebuffer_info.size.x, m_framebuffer_info.size.y);
	}

}
