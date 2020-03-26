#pragma once

#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics {

	class GLFrameBuffer : public FrameBuffer {
	private:
		unsigned int m_rbo;
		unsigned int m_id;

		GLTexture *m_texture;
		Sprite* m_sprite;

	public:
		GLFrameBuffer(const FrameBufferInfo& framebuffer_info);
		~GLFrameBuffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) override;

		virtual void bind_texture() override;
		virtual void unbind_texture() override;

		virtual const Sprite* get_sprite() override;
	
	};

}