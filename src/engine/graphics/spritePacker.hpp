#pragma once

#include "engine/utility/fileio.hpp"
#include "engine/internal_libs.hpp"
#include "sprite.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"



namespace oe::graphics
{
	struct __usr_data;

	// 2D only sprite packer
	class SpritePack
	{
	private:
		__usr_data* m_usr_data;

	private:
		int m_border;
		bool m_constructed;

		std::vector<Sprite*> m_sprites;

		Texture m_texture;

	public:
		// padding between textures
		SpritePack(int border = 3);
		~SpritePack();

		const Sprite* create(const oe::utils::image_data& sprite_texture);
		const Sprite* create(fs::path sprite_texture);
		const Sprite* emptySprite() const { return m_sprites.at(0); }
		
		void construct(); // sprite pack texture needs to be constructed before using it
		void constructRepeat(); // may be called multiple times, but will not delete imagedata

		void bind();
		Texture getTexture() const { return m_texture; }
	};

}