#include "textureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace graphics {

	TextureLoader::TextureLoader() {}

	unsigned int TextureLoader::loadTexture(const char *path, GLenum format) {

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else std::cout << "Failed to load texture!" << std::endl;
		stbi_image_free(data);

		mTextures.insert(std::pair<const char *, unsigned int>(path, texture));
		return texture;
	}

	unsigned int TextureLoader::getTexture(const char *path) {
		return getTexture(path, GL_RGBA);
	}

	unsigned int TextureLoader::getTexture(const char *path, GLenum format) {
		unsigned int find = 0;
		try {
			find = mTextures.at(path);
		}
		catch (...) {
			std::cout << "Slowload" << std::endl;
			return loadTexture(path, format);
		}
		std::cout << "Quickload" << std::endl;
		return find;
	}

}