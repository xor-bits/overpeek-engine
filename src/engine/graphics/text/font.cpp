#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>

#include "engine/internal_libs.h"



namespace oe::graphics {

	Font::Font(int resolution, bool manual_finish, std::string font_path) {
		spdlog::info("Loading font {} with resolution {}", font_path, resolution);
		m_resolution = resolution;

		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			oe_error_terminate("FT_Init_FreeType failed");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
			oe_error_terminate("Failed to load font at path {}", font_path.c_str());
			return;
		}

		//												  r, g, b, a
		size_t data_per_glyph = (size_t)resolution * (size_t)resolution * 4ul;
		size_t data_size = CHAR_COUNT * data_per_glyph;
		unsigned char* data = new unsigned char[data_size];
		memset(data, (unsigned char)0, data_size); // clear to 0

		bb_max_height = 0;
		bb_min_height = 0;

		FT_Set_Pixel_Sizes(face, 0, resolution);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		spdlog::debug("Loading {} glyphs", CHAR_COUNT);
		for (int i = 0; i < CHAR_COUNT; i++) {
			unsigned char c = i;

			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				spdlog::warn("Failed to load glyph: {}", c);
				m_glyphs[i] = nullptr; 
				continue;
			}

			auto g = face->glyph;

			if (!g->bitmap.buffer) { m_glyphs[i] = nullptr; continue; }

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

		m_ft = ft;
		m_face = face;
		m_data = data;

		if (!manual_finish) finish();
		spdlog::debug("Font loaded");
	}

	void Font::finish() {
		m_glyph_texture = Texture();
		m_glyph_texture.load3D(m_data, m_resolution, m_resolution, CHAR_COUNT);

		FT_Done_Face((FT_Face)m_face);
		FT_Done_FreeType((FT_Library)m_ft);
		delete[] m_data;
	}

	Font::~Font() {
		for (Glyph* g : m_glyphs) {
			if (g) delete g;
		}
	}

}