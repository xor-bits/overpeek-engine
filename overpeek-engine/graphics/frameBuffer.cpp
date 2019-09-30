#include "framebuffer.h"

#include "../utility/logger.h"

#include <GL/glew.h>

namespace oe {

	FrameBuffer::FrameBuffer(int width, int height) {
		glGenFramebuffers(1, &p_id);
		bind();
		p_width = width;
		p_height = height;

		// Texture
		p_texture = new Texture();
		p_texture->load2D(nullptr, width, height);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, p_texture->getId(), 0);

		// Render buffer object
		glGenRenderbuffers(1, &p_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, p_rbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_rbo);

		// FBO
		int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			Logger::error("Framebuffer not complete: " + fboStatus);

		clear();
		unbind();
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &p_id);
		glDeleteRenderbuffers(1, &p_rbo);
		delete p_texture;
	}

	void FrameBuffer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::clearColor(glm::vec4 c) {
		glClearColor(c.x, c.y, c.z, c.w);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, p_id);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}