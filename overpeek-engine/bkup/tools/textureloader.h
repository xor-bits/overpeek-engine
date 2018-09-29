#pragma once

#include <GL/glew.h>
#include <string>

namespace tools {

	class TextureLoader {
	public:
		static GLuint loadTexture(std::string filepath, GLenum format);
	};

}