#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <iostream>
#include <map>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

namespace graphics {

	class FontLoader {
	private:
		struct Character {
			GLuint textureID;	//Glyph texture id
			glm::ivec2 size;	//Glyph size
			glm::ivec2 bearing; //Offset from baseline to top left
			GLuint advance;		//Offset to advance to next glyph
		};
		std::map<GLchar, Character> mCharacters;

		GLuint mVAO, mVBO;

		Shader *mShader;

	private:

		bool init(std::string fontPath);

	public:
		FontLoader(std::string fontPath, Shader &shader);
		
		void renderText(std::string text, float x, float y, float scaleX, float scaleY, glm::vec3 color);
	};

}