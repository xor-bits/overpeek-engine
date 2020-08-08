#pragma once

#include "engine/internal_libs.hpp"
#include "engine/interfacegen.hpp"



namespace oe::graphics {

	// Higher level texture
	// 2D only
	struct Sprite {
		Texture m_owner;
		glm::vec2 position = { 0, 0 }; // position in packed texture
		glm::vec2 size = { 1, 1 };     // size in packed texture
	};

}