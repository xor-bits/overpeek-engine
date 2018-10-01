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

#define TEXT_ALIGN_LEFT 0
#define TEXT_ALIGN_CENTER 1
#define TEXT_ALIGN_RIGHT 2

#define TEXT_ALIGN_TOP 3
#define TEXT_ALIGN_BOTTOM 4

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

	private:

		bool init(std::string fontPath);

	public:
		FontLoader(std::string fontPath);
		
		void renderText(Shader *shader, std::string text, float x, float y, float w, float h, glm::vec3 color, int textAlignmentX, int textAlignmentY);
	};

}