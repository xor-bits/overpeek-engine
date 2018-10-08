#pragma once

#include <map>
#include <string>
#include "window.h"

#define MAX_TEXTURES 8

namespace graphics {

	class TextureLoader {
	private:
		unsigned int mTextures[MAX_TEXTURES];

	public:
		unsigned int loadTexture(std::string path, GLenum format, int id);
		unsigned int getTexture(int id);

	};

}