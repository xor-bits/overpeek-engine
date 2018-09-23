#include "textureManager.h"
#include "../../tools/textureloader.h"

#include <iostream>

GLuint TextureManager::m_textures[MAX_TEXTURES];

void TextureManager::loadTexture(std::string filepath, GLenum format, int id) {
	GLuint texture = tools::TextureLoader::loadTexture(filepath, format);
	m_textures[id] = texture;
}

GLuint TextureManager::getTexture(int id) {
	return m_textures[id];
}