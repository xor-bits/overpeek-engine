#pragma once

#include <map>
#include <string>
#include "window.h"

#define MAX_TEXTURES 8

namespace graphics {

	class TextureLoader {
	private:
		static unsigned int mTextures[MAX_TEXTURES];

	public:
		/*DEFAULT FORMAT IS GL_RGBA*/
		static unsigned int loadTexture(std::string path, GLenum format, int id);
		static unsigned int getTexture(int id);

	};

}