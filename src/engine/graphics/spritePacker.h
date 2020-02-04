#pragma once

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"
#include "sprite.h"
#include "gltexture.h"



namespace oe::graphics {


	// 2D only sprite packer
	class SpritePack {
	private:
		void* m_usr_data;

	private:
		glm::vec2 m_size;     // size of the packed texture
		int m_border;

		std::vector<std::vector<Sprite*>> m_sprites;

		Texture* m_texture;

	public:
		// padding between textures
		SpritePack(int border = 3);
		~SpritePack();

		// void construct(); will delete all data in these sprite images
		const Sprite* addSprite(oe::utils::image_data sprite_texture);
		const Sprite* addSprite(fs::path sprite_texture);

		// void construct(); will delete all data in these sprite images
		std::vector<std::vector<const Sprite*>> addMultiSprite(oe::utils::image_data sprite_texture, const glm::vec2& sprite_count);
		std::vector<std::vector<const Sprite*>> addMultiSprite(fs::path sprite_texture, const glm::vec2& sprite_count);
		
		// sprite pack texture needs to be constructed before using it
		void construct();

		// may be called multiple times, but increases memory usage
		void constructRepeat();

		void bind() const;

		const Sprite* empty_sprite() const { return m_sprites.at(0).at(0); }
	};

}