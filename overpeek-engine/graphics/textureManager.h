#pragma once

#include <map>
#include <string>

#define MAX_TEXTURES 8

namespace oe {

	class TextureManager {
	private:
		static unsigned int mTextures[MAX_TEXTURES];

	public:
		//Format must be RGBA
		static unsigned int loadTexture(std::string path, int id);
		
		//Format must be RGBA
		//16x16 array of r*r sized textures
		static unsigned int loadTextureAtlas(std::string path, int id, unsigned int r);
		
		static unsigned int getTexture(int id);
		
		static void saveTexture(std::string path, unsigned char *data, int width, int height, bool preview);

	};

}