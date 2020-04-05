#pragma once

#include "engine/internal_libs.hpp"



namespace oe::graphics {

	class SpritePack;

	// Higher level texture
	// 2D only
	class Sprite {
	private:
		SpritePack* m_sprite_pack;

	public:
		glm::vec2 position = { 0, 0 }; // position in packed texture
		glm::vec2 size = { 1, 1 };     // size in packed texture



		Sprite(SpritePack* sprite_pack)
			: m_sprite_pack(sprite_pack) {}
	};

}