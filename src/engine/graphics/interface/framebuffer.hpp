#pragma once

#include "engine/graphics/vertexData.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/enum.hpp"



namespace oe::graphics
{
	class IWindow;
	struct state { virtual ~state() {} };

	class IFrameBuffer
	{
	protected:
		FrameBufferInfo m_framebuffer_info;
		Sprite sprite;

	public:
		IFrameBuffer(const FrameBufferInfo& framebuffer_info);
		virtual ~IFrameBuffer();

		virtual void bind() = 0; // will bind corresponding texture
		virtual void clear(const oe::color& c = oe::colors::clear_color) = 0;

		static std::unique_ptr<state> save_state();
		static void load_state(const std::unique_ptr<state>&);
		static void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, const std::array<VertexData, 4>& vertices, size_t count);

		const Sprite& getSprite() { return sprite; }

	public:
		virtual Texture& getTexture() = 0;
		inline const FrameBufferInfo& getFrameBufferInfo() { return m_framebuffer_info; }

	};

}