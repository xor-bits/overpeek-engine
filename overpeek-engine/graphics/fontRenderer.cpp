#include "fontRenderer.h"
#include "../engine.h"


#include <vector>

#define FONT_RESOLUTION 48.0

namespace graphics {
	
	FontRenderer::FontRenderer(std::string &fontPath, QuadRenderer *renderer) {
		init(fontPath);
		initalized = true;

		m_renderer = renderer;
	}

	bool FontRenderer::init(std::string &fontPath) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			tools::Logger::error("Failed to initialize Freetype!");
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
		
		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
			tools::Logger::error("Failed to load font!");
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
				tools::Logger::error(std::string("Failed to load glyph for: \"") + (char)c + "\"");
				continue;
			}

			auto g = face->glyph;

			if (!g->bitmap.buffer) continue;

			//Now store character for later use
			Character *character = new Character {
				c,
				glm::ivec2(g->bitmap.width, g->bitmap.rows),
				glm::ivec2(g->bitmap_left,  g->bitmap_top),
				g->advance.x
			};
			m_characters.insert(std::pair<GLchar, Character*>(c, character));

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

	void FontRenderer::beginRendering() {
		m_renderer->beginRendering();
	}

	void FontRenderer::stopRendering() {
		m_renderer->stopRendering();
	}

	void FontRenderer::draw() {
		//Flush text
		m_renderer->draw(texture);
	}
	
	void FontRenderer::renderText(float _x, float _y, float _w, float _h, std::string &_text, glm::vec3 _color, int _textOriginX, int _textOriginY)
	{
		if (!initalized) return;
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
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w;
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
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w;
			}
			x -= textWidth/2.0;
		}
		
		if (_textOriginY == TEXT_ORIGIN_BOTTOM) {} //Already
		else if (_textOriginY == TEXT_ORIGIN_TOP) y += 1.0f * _h;
		else if (_textOriginY == TEXT_ORIGIN_CENTER) y += 0.25f * _h;
		
		// Iterate through all characters
		for (c = _text.begin(); c != _text.end(); c++)
		{
			if (*c == ' ') {
				x += _w / 2.0;
				continue;
			}
			GLfloat xpos = x + m_characters[*c]->bearing.x / FONT_RESOLUTION * _w;
			GLfloat ypos = y - m_characters[*c]->bearing.y / FONT_RESOLUTION * _h;
			GLfloat w = m_characters[*c]->size.x / FONT_RESOLUTION * _w;
			GLfloat h = m_characters[*c]->size.y / FONT_RESOLUTION * _h;

			m_renderer->renderBox(glm::vec2(xpos, ypos), glm::vec2(_w, _h), m_characters[*c]->textureID, glm::vec4(_color, 1.0));
		
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
}