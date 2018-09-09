#pragma once

#include <map>
#include "window.h"

namespace graphics {

	class TextureLoader {
	private:
		//Var
		std::map<const char *, unsigned int> mTextures;

		//Func
		unsigned int loadTexture(const char *path, GLenum format);
	public:
		//Func
		TextureLoader();
		
		/*DEFAULT FORMAT IS GL_RGBA*/
		unsigned int getTexture(const char *path);

		unsigned int getTexture(const char *path, GLenum format);
	};

}