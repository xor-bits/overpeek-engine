#include "gl_framebuffer.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/formatted_error.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>



namespace oe::graphics
{
	uint32_t GLFrameBuffer::bound_fbo_id = 0xFFFFFFFF;

	GLFrameBuffer::GLFrameBuffer(const FrameBufferInfo& framebuffer_info)
		: IFrameBuffer(framebuffer_info) 
	{
		oe_debug_call("gl_framebuffer");

		int32_t last_bound_fbo;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &last_bound_fbo);
		int32_t last_viewport_dim[4];
		glGetIntegerv(GL_VIEWPORT, &last_viewport_dim[0]);

		glGenFramebuffers(1, &m_id);
		GLFrameBuffer::bind();

		// Texture
		TextureInfo texture_info = {};
		texture_info.empty = true;
		texture_info.size_offset = { { framebuffer_info.size.x, 0}, { framebuffer_info.size.y, 0 } };
		texture_info.wrap = oe::texture_wrap::clamp_to_edge;
		texture_info.filter = oe::texture_filter::linear;
		m_texture = Texture(texture_info);
		// glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dynamic_cast<GLTexture*>(m_texture.get())->getGLTexture(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dynamic_cast<GLTexture*>(m_texture.get())->getGLTarget(), dynamic_cast<GLTexture*>(m_texture.get())->getGLTexture(), 0);
		
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
			throw oe::utils::formatted_error("Framebuffer not complete {}", fboStatus);

		GLFrameBuffer::clear();
		
		sprite.m_owner = m_texture;
		sprite.position = { 0.0f, 1.0f };
		sprite.size = { 1.0f, -1.0f };
		sprite.opacity = true;
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);
	}

	void GLFrameBuffer::clear(const glm::vec4& color)
	{
		GLFrameBuffer::bind();

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
