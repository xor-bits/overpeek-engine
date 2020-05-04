#pragma once

#include "engine/graphics/interface/instance.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class Renderer;
	class FrameBuffer {
	protected:
		FrameBufferInfo m_framebuffer_info;

	public:
		FrameBuffer(const FrameBufferInfo& framebuffer_info, Window* window);
		virtual ~FrameBuffer();

		virtual void bind() = 0; // will bind corresponding texture

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;

		// lacks implementation
		static void multipass(graphics::FrameBuffer& fb_0, graphics::FrameBuffer& fb_1, graphics::Renderer* renderer, size_t count);

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

}