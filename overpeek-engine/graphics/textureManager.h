#pragma once

#include <map>
#include <string>

#define MAX_TEXTURES 8

namespace oe {

	class TextureManager {
	private:
		static unsigned int mTextures[MAX_TEXTURES];

	public:
		static unsigned int loadTexture(std::string path, unsigned int format, int id);
		static unsigned int loadTextureAtlas(std::string path, unsigned int format, int id);
		static unsigned int getTexture(int id);
		static void saveTexture(std::string path, unsigned char *data, int width, int height, bool preview);

	};

}