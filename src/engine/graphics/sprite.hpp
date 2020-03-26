#pragma once

#include "engine/internal_libs.hpp"



namespace oe::graphics {

	class SpritePack;

	// Higher level texture
	// 2D only
	class Sprite {
	private:
		Texture* m_texture;

	public:
		glm::vec2 position = { 0, 0 }; // position in packed texture
		glm::vec2 size = { 1, 1 };     // size in packed texture

		Sprite(Texture* texture)
			: m_texture(texture) {}

		void bind_texture() {
			m_texture->bind();
		}

		void unbind_texture() {
			m_texture->unbind();
		}
	};

}