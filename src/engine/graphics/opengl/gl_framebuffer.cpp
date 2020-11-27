#include "gl_framebuffer.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/utility/formatted_error.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>



namespace oe::graphics
{
	uint32_t GLFrameBuffer::bound_fbo_id = 0xFFFFFFFF;
	glm::ivec4 GLFrameBuffer::current_viewport = { 0, 0, 0, 0 };

	struct gl_state : state
	{
		uint32_t fbo_id = 0;
		glm::ivec4 viewport = { 0, 0, 0, 0 };
		
		constexpr gl_state(uint32_t _fbo_id = 0, const glm::ivec4& _viewport = { 0, 0, 0, 0 })
			: fbo_id(_fbo_id), viewport(_viewport) {}
		~gl_state() override = default;
	};



	GLFrameBuffer::GLFrameBuffer(const FrameBufferInfo& framebuffer_info)
		: IFrameBuffer(framebuffer_info) 
	{
		int32_t last_bound_fbo = bound_fbo_id;
		glm::ivec4 last_viewport_dim = current_viewport;

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
		bind_fb(last_bound_fbo, last_viewport_dim);
		
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

	void GLFrameBuffer::clear(const oe::color& c)
	{
		GLFrameBuffer::bind();

		glClearColor(c.x, c.y, c.z, c.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLFrameBuffer::bind()
	{
		bind_fb(m_id, { 0, 0, m_framebuffer_info.size.x, m_framebuffer_info.size.y });
	}

	std::unique_ptr<state> GLFrameBuffer::save_state()
	{
		return std::make_unique<gl_state>(bound_fbo_id, current_viewport);
	}
	
	void GLFrameBuffer::load_state(const std::unique_ptr<state>& base_state)
	{
		gl_state* state = dynamic_cast<gl_state*>(base_state.get());
		bind_fb(state->fbo_id, state->viewport);
	}
	
	void GLFrameBuffer::bind_fb(uint32_t fb_id, const glm::ivec4& viewport)
	{
		if (bound_fbo_id != fb_id)
		{
			bound_fbo_id = fb_id;
			glBindFramebuffer(GL_FRAMEBUFFER, fb_id);
		}
		if(current_viewport != viewport)
		{
			current_viewport = viewport;
			glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		}
	}

}
