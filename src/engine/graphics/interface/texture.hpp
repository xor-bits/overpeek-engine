#pragma once

#include "engine/enum.h"
#include "engine/graphics/interface/instance.h"



namespace oe::graphics {

	class Texture {
	protected:
		TextureInfo m_texture_info;

	public:
		Texture(const Instance* instance, const TextureInfo& texture_info);
		~Texture();

		virtual void setData(const TextureInfo& texture_info) = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

	public:
		inline const TextureInfo& getTextureInfo() { return m_texture_info; }

	};

}