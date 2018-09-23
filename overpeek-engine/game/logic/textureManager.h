#pragma once

#include <map>
#include <GL/glew.h>

#define MAX_TEXTURES 8

class TextureManager {
private:
	static GLuint m_textures[MAX_TEXTURES];

public:
	static void loadTexture(std::string filepath, GLenum format, int id);
	static GLuint getTexture(int id);

};