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

	public:
		GLFrameBuffer(const FrameBufferInfo& framebuffer_info, Window* window);
		~GLFrameBuffer();

		virtual void bind() override;

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) override;

		virtual const Texture* getTexture() const override {
			return m_texture;
		}
	
	};

}