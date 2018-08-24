#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

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

	private:

		bool init(const char *fontPath);

	public:
		FontLoader(const char *fontPath, const char *vertShaderPath, const char *fragShaderPath);
		
		void renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	};

}