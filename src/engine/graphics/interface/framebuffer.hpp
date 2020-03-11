#pragma once

#include "engine/graphics/sprite.hpp"
#include "engine/graphics/interface/instance.hpp"
#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class FrameBuffer {
	protected:
		FrameBufferInfo m_framebuffer_info;

	public:
		FrameBuffer(const FrameBufferInfo& framebuffer_info);
		~FrameBuffer();

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;

		virtual void bind_texture() = 0;
		virtual void unbind_texture() = 0;

		virtual const Sprite* get_sprite() = 0;

	public:
		inline const FrameBufferInfo& getFrameBufferInfo() { return m_framebuffer_info; }

	};

}