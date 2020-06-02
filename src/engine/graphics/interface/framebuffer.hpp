#pragma once

#include "engine/graphics/interface/instance.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class Window;
	class FrameBuffer {
	protected:
		FrameBufferInfo m_framebuffer_info;

	public:
		FrameBuffer(const FrameBufferInfo& framebuffer_info, Window* window);
		virtual ~FrameBuffer();

		virtual void bind() = 0; // will bind corresponding texture

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;

		static void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, Window* window, const std::array<VertexData, 4>& vertices, size_t count);

		Sprite getSprite()
		{
			auto sprite = Sprite(getTexture());
			sprite.position = { 0.0f, 1.0f };
			sprite.size = { 1.0f, -1.0f };
			return sprite;
		}

	public:
		virtual const Texture* getTexture() const = 0;
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

		void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, Window* window, const std::array<VertexData, 4>& vertices, size_t count);

	private:
		void* m_primitive_renderer;

	};

}