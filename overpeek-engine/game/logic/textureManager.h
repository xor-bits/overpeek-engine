#pragma once

#include <map>
#include <GL/glew.h>

#define MAX_TEXTURES 8

class TextureManager {
private:
	static std::map<int, GLuint> m_textures;

public:
	static void loadTexture(std::string filepath, int id);
	static GLuint getTexture(int id);

};