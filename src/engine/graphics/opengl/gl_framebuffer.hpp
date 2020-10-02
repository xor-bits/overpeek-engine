#pragma once

#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics
{
	class GLFrameBuffer : public IFrameBuffer
	{
	private:
		unsigned int m_rbo;
		unsigned int m_id;

		Texture m_texture;
	
	public:
		static uint32_t bound_fbo_id;

	public:
		GLFrameBuffer(const FrameBufferInfo& framebuffer_info);
		~GLFrameBuffer();

		virtual void bind() override;
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) override;

		virtual Texture& getTexture() override {
			return m_texture;
		}
	
	};

}