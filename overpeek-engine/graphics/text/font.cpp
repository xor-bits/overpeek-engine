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

		//					char							r, g, b, a
		size_t data_size = 256 * resolution * resolution * 4;
		unsigned char* data = new unsigned char[data_size];
		//memset(data, (unsigned char)0, data_size); // clear to 0

		FT_Set_Pixel_Sizes(face, 0, resolution);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (int i = 0; i < 256; i++) {
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
				glm::vec2(g->bitmap_left,  g->bitmap_top) / (float)m_resolution,
				glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution
			};
			m_glyphs[c] = glyph;

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

		// instead of nullptr for some characters
		// do something
		for (int i = 0; i < 256; i++) {
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


		m_glyph_texture = Texture();
		m_glyph_texture.load3D(data, resolution, resolution, 256);

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