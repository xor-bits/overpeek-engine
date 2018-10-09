#include "textureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace graphics {

	unsigned int TextureManager::mTextures[MAX_TEXTURES];

	unsigned int TextureManager::loadTexture(std::string path, GLenum format, int id) {

		const int dataSize = 4 * 16 * 16 * 256;

		int width, height, nrChannels;
		GLubyte *tmpdata = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		GLubyte data[dataSize];
		for (int i = 0; i < 256; i++) {
			for (int y = 0; y < 16; y++) {
				for (int x = 0; x < 16; x++) {

					int tmpdataX = x + (i % 16 * 16);
					int tmpdataY = y * 256;
					int tmpdataI = floor(i / 16) * 4096;

					data[4 * (x + y * 16 + i * 256) + 0] = *(tmpdata + 4 * (tmpdataX + tmpdataY + tmpdataI) + 0);
					data[4 * (x + y * 16 + i * 256) + 1] = *(tmpdata + 4 * (tmpdataX + tmpdataY + tmpdataI) + 1);
					data[4 * (x + y * 16 + i * 256) + 2] = *(tmpdata + 4 * (tmpdataX + tmpdataY + tmpdataI) + 2);
					data[4 * (x + y * 16 + i * 256) + 3] = *(tmpdata + 4 * (tmpdataX + tmpdataY + tmpdataI) + 3);
				}
			}
		}
		stbi_image_free(tmpdata);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 16, 16, 256);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, 256, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		mTextures[id] = texture;
		return texture;
	}

	unsigned int TextureManager::getTexture(int id) {
		return mTextures[id];
	}

}