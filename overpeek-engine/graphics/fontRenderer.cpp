#include "fontRenderer.h"

#include <vector>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>
#include <GL/glew.h>

#include "../utility/logger.h"
#include "shader.h"
#include "buffers/vertexArray.h"
#include "renderer.h"
#include "quadRenderer.h"
#include "window.h"
#include "../math/math.h"

namespace oe {

	glm::vec4 FontRenderer::colors[9];
	
	FontRenderer::FontRenderer(const char *fontPath, QuadRenderer *renderer) {
		init(fontPath);
		initalized = true;

		m_renderer = renderer;


		//Colors
		colors[0] = glm::vec4(1.0);
		colors[1] = glm::vec4(0.5, 0.5, 0.5, 1.0);
		colors[2] = glm::vec4(0.0, 0.0, 0.0, 1.0);
		colors[3] = glm::vec4(1.0, 0.0, 0.0, 1.0);
		colors[4] = glm::vec4(0.0, 1.0, 0.0, 1.0);
		colors[5] = glm::vec4(0.0, 0.0, 1.0, 1.0);
		colors[6] = glm::vec4(0.0, 1.0, 1.0, 1.0);
		colors[7] = glm::vec4(1.0, 0.5, 0.0, 1.0);
		colors[8] = glm::vec4(1.0, 1.0, 0.0, 1.0);
	}

	bool FontRenderer::init(const char *fontPath) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			oe::Logger::out(oe::error, "Failed to initialize Freetype!");
			Window::terminate();
		}
		
		FT_Face face;
		if (FT_New_Face(ft, fontPath, 0, &face)) {
			oe::Logger::out(oe::error, "Failed to load font!");
			Window::terminate();
		}
	

		GLubyte *data = new GLubyte[128 * pow(FONT_RESOLUTION + FONT_BACKGROUND * 4, 2) * 4];
		for (int c = 0; c < 128 * pow(FONT_RESOLUTION + FONT_BACKGROUND * 4, 2) * 4; c++) {
			data[c] = 0;
		}
		GLuint maxWidth = FONT_RESOLUTION + FONT_BACKGROUND * 4, maxHeight = FONT_RESOLUTION + FONT_BACKGROUND * 4;
		
		FT_Set_Pixel_Sizes(face, 0, FONT_RESOLUTION);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		for (GLubyte c = 32; c < 127; c++) {
			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				oe::Logger::out(oe::error, "Failed to load glyph for: \"", (char*)c, "\"");
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

			//BLACK BACKGROUND
			for (int y = 0; y < g->bitmap.rows; y++)
			{
				for (int x = 0; x < g->bitmap.width; x++)
				{
					int pixel = (x + FONT_BACKGROUND / 4 * 1) + (y + FONT_BACKGROUND / 4 * 1) * maxWidth + c * maxWidth * maxHeight;
					data[pixel * 4 + 3] = oe::clamp(data[pixel * 4 + 3] + g->bitmap.buffer[x + y * g->bitmap.width], 0, 255);
					pixel = (x + FONT_BACKGROUND / 4 * 2) + (y + FONT_BACKGROUND / 4 * 2) * maxWidth + c * maxWidth * maxHeight;
					data[pixel * 4 + 3] = oe::clamp(data[pixel * 4 + 3] + g->bitmap.buffer[x + y * g->bitmap.width], 0, 255);
					pixel = (x + FONT_BACKGROUND / 4 * 3) + (y + FONT_BACKGROUND / 4 * 3) * maxWidth + c * maxWidth * maxHeight;
					data[pixel * 4 + 3] = oe::clamp(data[pixel * 4 + 3] + g->bitmap.buffer[x + y * g->bitmap.width], 0, 255);
					pixel = (x + FONT_BACKGROUND / 4 * 4) + (y + FONT_BACKGROUND / 4 * 4) * maxWidth + c * maxWidth * maxHeight;
					data[pixel * 4 + 3] = oe::clamp(data[pixel * 4 + 3] + g->bitmap.buffer[x + y * g->bitmap.width], 0, 255);
					
					//GLubyte byte = 0; data[pixel * 4 + 0] = byte; data[pixel * 4 + 1] = byte; data[pixel * 4 + 2] = byte; //Black color

				}
			}
			//WHITE FRONT FONT
			for (int y = 0; y < g->bitmap.rows; y++)
			{
				for (int x = 0; x < g->bitmap.width; x++)
				{
					int pixel = x + y * maxWidth + c * maxWidth * maxHeight;

					data[pixel * 4 + 0] = g->bitmap.buffer[x + y * g->bitmap.width];
					data[pixel * 4 + 1] = g->bitmap.buffer[x + y * g->bitmap.width];
					data[pixel * 4 + 2] = g->bitmap.buffer[x + y * g->bitmap.width];
					data[pixel * 4 + 3] = oe::clamp(data[pixel * 4 + 3] + g->bitmap.buffer[x + y * g->bitmap.width], 0, 255);
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
		return false;
	}

	void FontRenderer::beginRendering() {
		m_renderer->beginRendering();
	}

	void FontRenderer::stopRendering() {
		m_renderer->stopRendering();
	}

	void FontRenderer::draw() {
		//Flush text
		m_renderer->draw(texture, GL_TEXTURE_2D_ARRAY);
	}
	
	void FontRenderer::renderText(glm::vec3 pos, glm::vec2 scale, const char *_text, int _textOrigin)
	{
		std::string text = _text;
		if (!initalized) return;
		float x = pos.x;
		float y = pos.y;
		std::string::const_iterator c;

		glm::vec4 cur_color = glm::vec4(1.0);
		bool next_is_color = false;
		
		if (_textOrigin == topLeft || _textOrigin == centerLeft || _textOrigin == bottomLeft) {} //Already
		else if (_textOrigin == topRight || _textOrigin == centerRight || _textOrigin == bottomRight) {
			float textWidth = 0.0f;
			for (c = text.begin(); c != text.end(); c++)
			{
				if (*c < 32 || *c > 126 || next_is_color) {
					next_is_color = false;
					continue;
				}
				if (*c == '$') {
					next_is_color = true;
					continue;
				}

				if (*c == ' ') {
					textWidth += scale.x / 2.0;
					continue;
				}
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * scale.x;
			}
			x -= textWidth;
		}
		else { //Center
			float textWidth = 0.0f;
			for (c = text.begin(); c != text.end(); c++)
			{
				if (*c < 32 || *c > 126 || next_is_color) {
					next_is_color = false;
					continue;
				}
				if (*c == '$') {
					next_is_color = true;
					continue;
				}

				if (*c == ' ') {
					textWidth += scale.x / 2.0;
					continue;
				}
				textWidth += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * scale.x;
			}
			x -= textWidth/2.0;
		}
		
		if (_textOrigin == bottomLeft || _textOrigin == bottomCenter || _textOrigin == bottomRight) {} //Already
		else if (_textOrigin == topLeft || _textOrigin == topCenter || _textOrigin == topRight) y += 1.0f * scale.y;
		else if (_textOrigin == centerLeft || _textOrigin == center || _textOrigin == centerRight) y += 0.25f * scale.y;
		
		// Iterate through all characters
		for (c = text.begin(); c != text.end(); c++)
		{
			if (*c < 32 || *c > 126) continue;
			if (next_is_color) {
				char color_index[] = { *c, NULL };
				int index = (int)atof(color_index);

				if (oe::isInRange(index, 0, 8)) {
					cur_color = colors[index];
				}
				
				next_is_color = false;
				continue;
			}
			if (*c == '$') {
				next_is_color = true;
				continue;
			}

			if (*c == ' ') {
				x += scale.x / 2.0;
				continue;
			}
			float xpos = x + m_characters[*c]->bearing.x / FONT_RESOLUTION * scale.x;
			float ypos = y - m_characters[*c]->bearing.y / FONT_RESOLUTION * scale.y;
			float w = ((m_characters[*c]->advance >> 6) - FONT_BACKGROUND) / FONT_RESOLUTION * scale.x;
			float h = m_characters[*c]->size.y / FONT_RESOLUTION * scale.y;

			VertexData p = VertexData(glm::vec3(xpos, ypos, 0.0f), glm::vec2(0.0f, 0.0f), m_characters[*c]->textureID, cur_color);
			m_renderer->submitVertex(p);

			p = VertexData(glm::vec3(xpos, ypos + h, 0.0f), glm::vec2(0.0f, m_characters[*c]->size.y / FONT_RESOLUTION), m_characters[*c]->textureID, cur_color);
			m_renderer->submitVertex(p);

			p = VertexData(glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec2(((m_characters[*c]->advance >> 6) - FONT_BACKGROUND) / FONT_RESOLUTION, m_characters[*c]->size.y / FONT_RESOLUTION), m_characters[*c]->textureID, cur_color);
			m_renderer->submitVertex(p);

			p = VertexData(glm::vec3(xpos + w, ypos, 0.0f), glm::vec2(((m_characters[*c]->advance >> 6) - FONT_BACKGROUND) / FONT_RESOLUTION, 0.0f), m_characters[*c]->textureID, cur_color);
			m_renderer->submitVertex(p);

			//m_renderer->renderBox(glm::vec2(xpos, ypos), glm::vec2(_w, _h), m_characters[*c]->textureID, cur_color);
		
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (m_characters[*c]->advance >> 6) / FONT_RESOLUTION * scale.x; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
}