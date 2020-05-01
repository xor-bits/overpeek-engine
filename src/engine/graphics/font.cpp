#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"



namespace oe::graphics {

	Font::Font(oe::graphics::SpritePack* sprite_packer, int resolution, std::string font_path) {
		oe_debug_call("font");

		m_resolution = resolution;

		// Freetype library
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			oe_error_terminate("FT_Init_FreeType failed");
			return;
		}

		// Font
		FT_Face face;
		if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
			oe_error_terminate("Failed to load font at path {}", font_path.c_str());
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, resolution);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		// all glyphs
		bb_max_height = 0; bb_min_height = 0;
		for (int i = 0; i < 128; i++) {
			unsigned char c = i;

			//Load glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				spdlog::warn("Failed to load glyph: {}", c);
				continue;
			}

			// glyph
			auto g = face->glyph;
			if (!g->bitmap.buffer) continue;

			// white font
			unsigned char* data = new unsigned char[g->bitmap.width * g->bitmap.rows * 4]();
			for (int y = 0; y < g->bitmap.rows; y++) {
				for (int x = 0; x < g->bitmap.width; x++) {
					int pixel = (x + y * g->bitmap.width) * 4;

					data[pixel + 0] = 255;
					data[pixel + 1] = 255;
					data[pixel + 2] = 255;
					data[pixel + 3] = g->bitmap.buffer[x + y * g->bitmap.width];
				}
			}

			//Now store character for later use
			Glyph* glyph = new Glyph{
				c,
				glm::vec2(g->bitmap.width, g->bitmap.rows) / (float)m_resolution,
				glm::vec2(g->bitmap_left, -g->bitmap_top) / (float)m_resolution,
				glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution,

				// add glyph to sprite packer
				sprite_packer->addSprite(oe::utils::image_data(data, oe::formats::rgba, g->bitmap.width, g->bitmap.rows))
			};
			m_glyphs.insert(std::make_pair(c, glyph));

			// find the max bearing y
			float bearing_y = -glyph->top_left.y;
			if (bb_max_height < bearing_y) bb_max_height = bearing_y;

			// find the lowest point
			float lowest_y = (glyph->top_left.y + glyph->size.y);
			if (bb_min_height > lowest_y) bb_min_height = lowest_y;

			delete[] data;
		}

		// whitespace
		Glyph* glyph = new Glyph{
			' ',
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.3f),
			sprite_packer->empty_sprite()
		};
		m_glyphs.insert(std::make_pair(' ', glyph));

		bb_height = bb_max_height - bb_min_height;

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	Font::~Font() {
		for (auto& g : m_glyphs) {
			if (g.second) delete g.second;
		}
	}

}