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
#include "buffers/indexBuffer.h"
#include "buffers/buffer.h"
#include "buffers/vertexArray.h"

#define TEXT_ORIGIN_LEFT 0
#define TEXT_ORIGIN_CENTER 1
#define TEXT_ORIGIN_RIGHT 2

#define TEXT_ORIGIN_TOP 3
#define TEXT_ORIGIN_BOTTOM 4

#define TEXT_MAX_QUADS_PER_FLUSH 64000
#define TEXT_VERTEX_PER_QUAD 6 * 2
#define TEXT_MAX_VBO TEXT_MAX_QUADS_PER_FLUSH * TEXT_VERTEX_PER_QUAD

namespace graphics {

	class FontLoader {
	private:
		GLuint texture;
		struct Character {
			GLuint textureID;		//Glyph texture id
			glm::ivec2 size;	//Glyph size
			glm::ivec2 bearing; //Offset from baseline to top left
			GLuint advance;		//Offset to advance to next glyph
		};
		std::map<GLchar, Character> m_characters;

		VertexArray *m_VAO;
		Buffer *m_VBO;
		Buffer *m_UV;
		Buffer *m_ID;
		Buffer *m_COLOR;

		GLuint quadCount;
		GLfloat m_vertex[TEXT_MAX_VBO];
		GLfloat m_uv[TEXT_MAX_VBO];
		GLfloat m_id[TEXT_MAX_VBO];
		GLfloat m_color[TEXT_MAX_VBO];

	private:

		bool init(std::string fontPath);

	public:
		FontLoader(std::string fontPath);
		
		void renderText(float x, float y, float w, float h, std::string text, glm::vec3 color, int textOriginX, int textOriginY);
		void flush();
	};

}