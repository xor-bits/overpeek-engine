#pragma once

#include "engine/graphics/interface/framebuffer.h"
#include "engine/graphics/opengl/gl_texture.h"
#include "engine/internal_libs.h"



namespace oe::graphics {

	class GLFrameBuffer : public FrameBuffer {
	private:
		unsigned int m_rbo;
		unsigned int m_id;

		GLTexture *m_texture;

	public:
		GLFrameBuffer(const Instance* instance, const FrameBufferInfo& framebuffer_info);
		~GLFrameBuffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) override;
	
	};

}