#include "fontLoader.h"
#include "../engine.h"


#include <vector>

#define FONT_RESOLUTION 48.0

namespace graphics {
	
	FontLoader::FontLoader(std::string fontPath) {
		init(fontPath);

		quadCount = 0;
		for (int i = 0; i < TEXT_MAX_QUADS_PER_FLUSH; i++)
		{
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 0] = 0.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 1] = 1.0;

			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 2] = 1.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 3] = 1.0;

			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 4] = 1.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 5] = 0.0;

			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 6] = 0.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 7] = 1.0;

			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 8] = 1.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 9] = 0.0;

			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 10] = 0.0;
			m_uv[(i * TEXT_VERTEX_PER_QUAD) + 11] = 0.0;
		}

		m_VAO = new VertexArray();
		m_UV = new Buffer(m_uv, TEXT_MAX_VBO, 2, sizeof(GLfloat), GL_STATIC_DRAW);
		m_VBO = new Buffer(0, TEXT_MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_ID = new Buffer(0, TEXT_MAX_VBO, 2, sizeof(GLfloat), GL_DYNAMIC_DRAW);
		m_COLOR = new Buffer(0, TEXT_MAX_VBO*2, 4, sizeof(GLfloat), GL_DYNAMIC_DRAW);

		m_VAO->addBuffer(m_VBO, 0);
		m_VAO->addBuffer(m_UV, 1);
		m_VAO->addBuffer(m_ID, 2);
		m_VAO->addBuffer(m_COLOR, 3);
	}

	bool GetPixelValue(unsigned char *pBuffer, int iPitch, int x, int y)
	{
		unsigned char *pRow = &pBuffer[y*iPitch];
		char cValue = pRow[x >> 3];
		bool bRet = (cValue & (1 << (x & 7))) != 0;
		return bRet;
	}

	bool FontLoader::init(std::string fontPath) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "ERROR Initalizing FreeType!" << std::endl;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
		
		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
			std::cout << "ERROR loading font!" << std::endl;
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}

		GLubyte data[128 * (int)FONT_RESOLUTION * (int)FONT_RESOLUTION];
		for (int c = 0; c < 128 * FONT_RESOLUTION * FONT_RESOLUTION; c++) {
			data[c] = 0;
		}
		GLuint maxWidth = FONT_RESOLUTION, maxHeight = FONT_RESOLUTION;
		
		FT_Set_Pixel_Sizes(face, 0, FONT_RESOLUTION);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		for (GLubyte c = 0; c < 128; c++) {
			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "ERROR loading glyph for " << c << std::endl;
				continue;
			}

			auto g = face->glyph;

			if (!g->bitmap.buffer) continue;

			//Now store character for later use
			Character character = {
				c,
				glm::ivec2(g->bitmap.width, g->bitmap.rows),
				glm::ivec2(g->bitmap_left,  g->bitmap_top),
				g->advance.x
			};
			m_characters.insert(std::pair<GLchar, Character>(c, character));

			for (int x = 0; x < g->bitmap.width; x++)
			{
				for (int y = 0; y < g->bitmap.rows; y++)
				{
					int pixel = x + y * maxWidth + c * maxWidth * maxHeight;
					data[pixel] = g->bitmap.buffer[x + y * g->bitmap.width];
				}
			}
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R8, maxWidth, maxHeight, 128);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, maxWidth, maxHeight, 128, GL_RED, GL_UNSIGNED_BYTE, data);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void FontLoader::flush() {
		//Flush text
		
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		m_VAO->bind();

		m_VBO->setBufferData(m_vertex, quadCount * TEXT_VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_ID->setBufferData(m_id, quadCount * TEXT_VERTEX_PER_QUAD, 2, sizeof(GLfloat));
		m_COLOR->setBufferData(m_color, quadCount * TEXT_VERTEX_PER_QUAD * 2, 4, sizeof(GLfloat));

		glDrawArrays(GL_TRIANGLES, 0, quadCount * 6);

		quadCount = 0;
	}
	
	void FontLoader::renderText(float _x, float _y, float _w, float _h, std::string _text, glm::vec3 _color, int _textOriginX, int _textOriginY)
	{
		GLfloat x = _x;
		GLfloat y = _y;
		std::string::const_iterator c;
		
		if (_textOriginX == TEXT_ORIGIN_LEFT) {} //Already
		else if (_textOriginX == TEXT_ORIGIN_RIGHT) {
			GLfloat textWidth = 0.0f;
			for (c = _text.begin(); c != _text.end(); c++)
			{
				if (*c == ' ') {
					textWidth += _w / 2.0;
					continue;
				}
				textWidth += (m_characters[*c].advance >> 6) / FONT_RESOLUTION * _w;
			}
			x -= textWidth;
		}
		else if (_textOriginX == TEXT_ORIGIN_CENTER) {
			GLfloat textWidth = 0.0f;
			for (c = _text.begin(); c != _text.end(); c++)
			{
				if (*c == ' ') {
					textWidth += _w / 2.0;
					continue;
				}
				textWidth += (m_characters[*c].advance >> 6) / FONT_RESOLUTION * _w;
			}
			x -= textWidth/2.0;
		}
		
		if (_textOriginY == TEXT_ORIGIN_BOTTOM) {} //Already
		else if (_textOriginY == TEXT_ORIGIN_TOP) y += 1.0f * _h;
		else if (_textOriginY == TEXT_ORIGIN_CENTER) y += 0.25f * _h;
		
		// Iterate through all characters
		for (c = _text.begin(); c != _text.end(); c++)
		{
			Character ch = m_characters[*c];
			if (*c == ' ') {
				x += _w / 2.0;
				continue;
			}
			GLfloat xpos = x + ch.bearing.x / FONT_RESOLUTION * _w;
			GLfloat ypos = y - ch.bearing.y / FONT_RESOLUTION * _h;
			GLfloat w = ch.size.x / FONT_RESOLUTION * _w;
			GLfloat h = ch.size.y / FONT_RESOLUTION * _h;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 0] = xpos;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 1] = ypos + _h;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 2] = xpos + _w;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 3] = ypos + _h;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 4] = xpos + _w;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 5] = ypos;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 6] = xpos;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 7] = ypos + _h;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 8] = xpos + _w;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 9] = ypos;

			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 10] = xpos;
			m_vertex[(quadCount * TEXT_VERTEX_PER_QUAD) + 11] = ypos;

			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 0] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 1] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 2] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 3] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 4] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 5] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 6] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 7] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 8] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 9] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 10] = m_characters[*c].textureID;
			m_id[(quadCount * TEXT_VERTEX_PER_QUAD) + 11] = m_characters[*c].textureID;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 0] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 1] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 2] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 3] = 1.0;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 4] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 5] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 6] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 7] = 1.0;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 8] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 9] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 10] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 11] = 1.0;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 12] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 13] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 14] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 15] = 1.0;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 16] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 17] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 18] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 19] = 1.0;

			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 20] = _color.x;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 21] = _color.y;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 22] = _color.z;
			m_color[(quadCount * TEXT_VERTEX_PER_QUAD * 2) + 23] = 1.0;

			quadCount++;
		
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.advance >> 6) / FONT_RESOLUTION * _w; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
}