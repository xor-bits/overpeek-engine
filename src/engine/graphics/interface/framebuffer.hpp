#pragma once

#include "engine/graphics/interface/instance.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class IWindow;
	class IFrameBuffer {
	protected:
		FrameBufferInfo m_framebuffer_info;
		Sprite sprite;

	public:
		IFrameBuffer(const FrameBufferInfo& framebuffer_info, Window& window);
		virtual ~IFrameBuffer();

		virtual void bind() = 0; // will bind corresponding texture
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;

		static void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, Window& window, const std::array<VertexData, 4>& vertices, size_t count);

		const Sprite& getSprite() { return sprite; }

	public:
		virtual Texture& getTexture() = 0;
		inline const FrameBufferInfo& getFrameBufferInfo() { return m_framebuffer_info; }

	};

	class FramebufferMultipass
	{
	private:
		FramebufferMultipass();
		static FramebufferMultipass* fb_multipasser;

	public:
		static FramebufferMultipass& getSingleton()
		{
			if (!fb_multipasser) fb_multipasser = new FramebufferMultipass();
			return *fb_multipasser;
		}

		void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, Window& window, const std::array<VertexData, 4>& vertices, size_t count);

	private:
		void* m_primitive_renderer;

	};

}