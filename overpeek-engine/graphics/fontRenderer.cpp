#include "fontRenderer.h"

#include "../utility/logger.h"
#include "shader.h"
#include "buffers/vertexArray.h"
#include "renderer.h"
#include "quadRenderer.h"
#include "window.h"

#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>

namespace oe {
	
	FontRenderer::FontRenderer(const char *fontPath, QuadRenderer *renderer) {
		init(fontPath);
		initalized = true;

		m_renderer = renderer;
	}

	bool FontRenderer::init(const char *fontPath) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			oe::Logger::error("Failed to initialize Freetype!");
			Window::terminate();
		}
		
		FT_Face face;
		if (FT_New_Face(ft, fontPath, 0, &face)) {
			oe::Logger::error("Failed to load font!");
			Window::terminate();
		}
		

		GLubyte *data = new GLubyte[128 * (int)FONT_RESOLUTION * TEXTURE_SCALAR * (int)FONT_RESOLUTION * TEXTURE_SCALAR * 4];
		for (int c = 0; c < 128 * FONT_RESOLUTION * FONT_RESOLUTION; c++) {
			data[c] = 0;
		}
		GLuint maxWidth = FONT_RESOLUTION * TEXTURE_SCALAR, maxHeight = FONT_RESOLUTION * TEXTURE_SCALAR;
		
		FT_Set_Pixel_Sizes(face, 0, FONT_RESOLUTION);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		for (GLubyte c = 32; c < 127; c++) {
			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				oe::Logger::error(std::string("Failed to load glyph for: \"") + (char)c + "\"");
				continue;
			}

			auto g = face->glyph;

			if (!g->bitmap.buffer) continue;

			g->format = FT_GLYPH_FORMAT_OUTLINE;
			// Set up a stroker.
			FT_Stroker stroker;
			FT_Stroker_New(ft, &stroker);
			FT_Stroker_Set(stroker, FONT_BORDER, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

			FT_Glyph glyph; 
			if (FT_Get_Glyph(g, &glyph)) {
				oe::Logger::error("Glyph load fail");
			}
			if (FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1)) {
				oe::Logger::error("Stroke border fail");
			}

			glyph->format = FT_GLYPH_FORMAT_BITMAP;
			if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1)) {
				oe::Logger::error("Glyph to bitmap fail");
			}
			FT_BitmapGlyph bitmap;
			bitmap = (FT_BitmapGlyph)glyph;
			
			oe::Logger::info(glyph->advance.x);
			oe::Logger::info(g->advance.x);
			//Now store character for later use
			Character *character = new Character {
				//c,
				//glm::ivec2(g->bitmap.width, g->bitmap.rows),
				//glm::ivec2(g->bitmap_left,  g->bitmap_top),
				//g->advance.x
				c,
				glm::ivec2(bitmap->bitmap.width, bitmap->bitmap.rows),
				glm::ivec2(bitmap->left,  bitmap->top),
				(glyph->advance.x / 1024) + 128
			};
			m_characters.insert(std::pair<GLchar, Character*>(c, character));

			int xdiff = (float(bitmap->bitmap.width - g->bitmap.width) / 2.0 + 0.5);
			int ydiff = (float(bitmap->bitmap.rows - g->bitmap.rows) / 2.0 + 0.5);
			for (int x = 0; x < bitmap->bitmap.width; x++)
			{
				for (int y = 0; y < bitmap->bitmap.rows; y++)
				{
					int pixel = x + y * maxWidth + c * maxWidth * maxHeight;
					
					char byte = 0;
					if ((x - xdiff) >= 0 && (x - xdiff) < g->bitmap.width) {
						if ((y - ydiff) >= 0 && (y - ydiff) < g->bitmap.rows) {
							byte = g->bitmap.buffer[(x - xdiff) + (y - ydiff)* g->bitmap.width];
						}
					}
					data[pixel * 4 + 0] = byte;
					data[pixel * 4 + 1] = byte;
					data[pixel * 4 + 2] = byte;
					data[pixel * 4 + 3] = bitmap->bitmap.buffer[x + y * bitmap->bitmap.width];
				}
			}
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, maxWidth, maxHeight, 128);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, maxWidth, maxHeight, 128, GL_RGBA, GL_UNSIGNED_BYTE, data);

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
	
	void FontRenderer::renderText(float _x, float _y, float _w, float _h, const char *_text, glm::vec3 _color, int _textOrigin)
	{
		std::string text = _text;
		if (!initalized) return;
		float x = _x;
		float y = _y;
		std::string::const_iterator c;
		
		if (_textOrigin == topLeft || _textOrigin == centerLeft || _textOrigin == bottomLeft) {} //Already
		else if (_textOrigin == topRight || _textOrigin == centerRight || _textOrigin == bottomRight) {
			float textWidth = 0.0f;
			for (c = text.begin(); c != text.end(); c++)
			{
				if (*c == ' ') {
					textWidth += _w / 2.0;
					continue;
				}
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w;
			}
			x -= textWidth;
		}
		else { //Center
			float textWidth = 0.0f;
			for (c = text.begin(); c != text.end(); c++)
			{
				if (*c == ' ') {
					textWidth += _w / 2.0;
					continue;
				}
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w;
			}
			x -= textWidth/2.0;
		}
		
		if (_textOrigin == bottomLeft || _textOrigin == bottomCenter || _textOrigin == bottomRight) {} //Already
		else if (_textOrigin == topLeft || _textOrigin == topCenter || _textOrigin == topRight) y += 1.0f * _h;
		else if (_textOrigin == centerLeft || _textOrigin == center || _textOrigin == centerRight) y += 0.25f * _h;
		
		// Iterate through all characters
		for (c = text.begin(); c != text.end(); c++)
		{
			if (*c == ' ') {
				x += _w / 2.0;
				continue;
			}
			float xpos = x + m_characters[*c]->bearing.x / FONT_RESOLUTION * _w;
			float ypos = y - m_characters[*c]->bearing.y / FONT_RESOLUTION * _h;
			float w = m_characters[*c]->size.x / FONT_RESOLUTION * _w;
			float h = m_characters[*c]->size.y / FONT_RESOLUTION * _h;

			m_renderer->renderBox(glm::vec2(xpos, ypos), glm::vec2(_w, _h), m_characters[*c]->textureID, glm::vec4(_color, 1.0));
		
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * _w; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
}