#include "textureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace graphics {

	unsigned int TextureLoader::mTextures[MAX_TEXTURES];

	unsigned int TextureLoader::loadTexture(std::string path, GLenum format, int id) {

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else std::cout << "Failed to load texture!" << std::endl;
		stbi_image_free(data);

		mTextures[id] = texture;
		return texture;
	}

	unsigned int TextureLoader::getTexture(int id) {
		return mTextures[id];
	}

}