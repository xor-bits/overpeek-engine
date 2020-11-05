#pragma once

#include "engine/utility/fileio.hpp"
#include "engine/internal_libs.hpp"
#include "engine/interfacegen.hpp"
#include "engine/graphics/interface/texture.hpp"



namespace oe::graphics
{
	struct __usr_data;
	struct Sprite;

	// 2D only sprite packer
	class SpritePack
	{
	private:
		__usr_data* m_usr_data;

	private:
		int m_border;
		bool m_constructed;

		std::vector<std::unique_ptr<Sprite>> m_sprites;

		Texture m_texture;
		oe::formats m_format_limit;

		[[nodiscard]] inline static oe::TextureInfo default_texture_settings() noexcept
		{
			oe::TextureInfo info;
			info.generate_mipmaps = true;
			return info;
		}

	public:
		// padding between textures
		SpritePack(int border = 3);
		~SpritePack();

		const Sprite* create(oe::utils::image_data&& sprite_texture_move);
		const Sprite* create(const oe::utils::image_data& sprite_texture_copy);
		const Sprite* create(const oe::utils::FileIO& path);
		const Sprite* emptySprite() const { return m_sprites.at(0).get(); }

		void clear(); // will invalidate all Sprite pointers

		
		
		void construct(const oe::TextureInfo& texture_settings = default_texture_settings()); // sprite pack texture needs to be constructed before using it
		void constructRepeat(const oe::TextureInfo& texture_settings = default_texture_settings()); // may be called multiple times, but will not delete imagedata

		void bind();
		Texture getTexture() const { return m_texture; }
	};

}