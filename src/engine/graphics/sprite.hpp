#pragma once

#include "engine/internal_libs.hpp"



namespace oe::graphics {

	class Texture;

	// Higher level texture
	// 2D only
	class Sprite {
	public:
		Texture* m_owner;
		glm::vec2 position = { 0, 0 }; // position in packed texture
		glm::vec2 size = { 1, 1 };     // size in packed texture

		Sprite(Texture* texture)
			: m_owner(texture) {}
	};

}