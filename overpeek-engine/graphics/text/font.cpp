#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>



namespace oe {

	Font::Font(int resolution, std::string font_path) {
		spdlog::info("Loading font: {}, with resolution: {}", font_path, resolution);
		m_resolution = resolution;

		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			spdlog::error("Failed to initialize Freetype");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
			spdlog::error("Failed to load font");
			return;
		}

		//												  r, g, b, a
		size_t data_per_glyph = resolution * resolution * 4;
		size_t data_size = CHAR_COUNT * data_per_glyph;
		unsigned char* data = new unsigned char[data_size];
		//memset(data, (unsigned char)0, data_size); // clear to 0

		bb_max_height = 0;
		bb_min_height = 0;

		FT_Set_Pixel_Sizes(face, 0, resolution);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (int i = 0; i < CHAR_COUNT; i++) {
			unsigned char c = i;

			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				spdlog::warn("Failed to load glyph: {}", c);
				continue;
			}

			auto g = face->glyph;

			if (!g->bitmap.buffer) continue;

			//Now store character for later use
			Glyph* glyph = new Glyph{
				c,
				glm::vec2(g->bitmap.width, g->bitmap.rows) / (float)m_resolution,
				glm::vec2(g->bitmap_left, -g->bitmap_top) / (float)m_resolution,
				glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution
			};
			m_glyphs[c] = glyph;

			// find the max bearing y
			float bearing_y = -glyph->top_left.y;
			if (bb_max_height < bearing_y) bb_max_height = bearing_y;

			// find the lowest point
			float lowest_y = (glyph->top_left.y + glyph->size.y);
			if (bb_min_height > lowest_y) bb_min_height = lowest_y;

			//WHITE FRONT FONT
			for (int y = 0; y < g->bitmap.rows; y++)
			{
				for (int x = 0; x < g->bitmap.width; x++)
				{
					int pixel = (x + y * resolution + c * resolution * resolution) * 4;

					data[pixel + 0] = 255;
					data[pixel + 1] = 255;
					data[pixel + 2] = 255;
					data[pixel + 3] = g->bitmap.buffer[x + y * g->bitmap.width];
				}
			}
		}
		
		// set glyph at 1 to white box
		memset(data + data_per_glyph, 255, data_per_glyph);

		// instead of nullptr for some characters
		// do something
		for (int i = 0; i < CHAR_COUNT; i++) {
			if (!m_glyphs[i]) {
				m_glyphs[i] = new Glyph{
					(unsigned char)i,
					glm::vec2(0.0f),
					glm::vec2(0.0f),
					glm::vec2(0.0f)
				};
			}
		}
		// whitespace
		m_glyphs[32] = new Glyph{
			32,
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.3f)
		};

		bb_height = bb_max_height - bb_min_height;
		spdlog::debug("bb_max_height: {}, bb_min_height: {}, bb_height: {}", bb_max_height, bb_min_height, bb_height);

		m_glyph_texture = Texture();
		m_glyph_texture.load3D(data, resolution, resolution, CHAR_COUNT);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		delete[] data;
	}

	Font::~Font() {
		for (Glyph* g : m_glyphs) {
			delete g;
		}
	}

}