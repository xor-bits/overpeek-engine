#pragma once

#include "engine/enum.hpp"
#include "engine/graphics/interface/instance.hpp"



namespace oe::graphics {

	class ITexture {
	protected:
		TextureInfo m_texture_info;

	public:
		ITexture(const TextureInfo& texture_info);
		virtual ~ITexture();

		virtual void setData(const TextureInfo& texture_info) = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		// compute shader only
		virtual void bindCompute() const = 0;
		virtual void unbindCompute() const = 0;

	public:
		inline const TextureInfo& getTextureInfo() { return m_texture_info; }

	};

}