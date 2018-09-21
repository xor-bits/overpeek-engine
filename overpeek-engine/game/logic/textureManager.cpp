#include "textureManager.h"
#include "../../tools/textureloader.h"

#include <iostream>

std::map<int, GLuint> TextureManager::m_textures;

void TextureManager::loadTexture(std::string filepath, int id) {
	GLuint texture = tools::TextureLoader::loadTexture(filepath, GL_RGB);
	m_textures[id] = texture;
}

GLuint TextureManager::getTexture(int id) {
	return m_textures.find(id)->second;
}