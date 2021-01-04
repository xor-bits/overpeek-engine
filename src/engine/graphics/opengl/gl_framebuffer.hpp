#pragma once

#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"
#include "engine/internal_libs.hpp"



namespace oe::graphics
{
	class GLFrameBuffer : public IFrameBuffer
	{
	private:
		// uint32_t m_rbo;
		uint32_t m_id;

		Texture m_texture;
	
	public:
		static uint32_t bound_fbo_id;
		static glm::ivec4 current_viewport;
		static glm::ivec2 gl_max_fb_size;

		static void bind_fb(uint32_t fb_id, const glm::ivec4& viewport);
	
	public:
		GLFrameBuffer(const FrameBufferInfo& framebuffer_info);
		~GLFrameBuffer() override;

		void bind() override;
		void clear(const oe::color& c = oe::colors::clear_color) override;

		static std::unique_ptr<state> save_state();
		static void load_state(const std::unique_ptr<state>&);

		inline Texture& getTexture() override { return m_texture; }
	};

}