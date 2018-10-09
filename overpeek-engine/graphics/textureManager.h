#pragma once

#include <map>
#include <string>
#include "window.h"

#define MAX_TEXTURES 8

namespace graphics {

	class TextureManager {
	private:
		static unsigned int mTextures[MAX_TEXTURES];

	public:
		static unsigned int loadTexture(std::string path, GLenum format, int id);
		static unsigned int getTexture(int id);

	};

}