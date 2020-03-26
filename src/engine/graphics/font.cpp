#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"



namespace oe::graphics {

	Font::Glyph* load_glyph(char c, unsigned char* buffer, size_t width, size_t advance, const glm::ivec2& top_left, float resolution, SpritePack* sprite_packer) {
		// white font
		unsigned char* data = new unsigned char[width * resolution * 4]();
		for (int y = 0; y < resolution; y++) {
			for (int x = 0; x < width; x++) {
				int pixel = (x + y * width) * 4;

				data[pixel + 0] = 255;
				data[pixel + 1] = 255;
				data[pixel + 2] = 255;
				data[pixel + 3] = buffer[x + (top_left.y + y) * width];
			}
		}

		//Now store character for later use
		Font::Glyph* glyph = new Font::Glyph{
			c,
			width / resolution,
			(advance >> 6) / resolution,

			// add glyph to sprite packer
			sprite_packer->addSprite(oe::utils::loadImageMove(data, width, resolution))
		};
		return glyph;
	}

	Font::Font(oe::graphics::SpritePack* sprite_packer, int resolution, std::string font_path) {
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
		for (int i = 1; i < 128; i++) { // start from 1 because [0] will be empty box
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
			Glyph* glyph = load_glyph(c, g->bitmap.buffer, g->bitmap.width, g->advance.x, { g->bitmap_left, g->bitmap_top }, m_resolution, sprite_packer);
			m_glyphs.insert(std::make_pair(c, glyph));

			// no need to delete[] data, sprite packer will delete them after it is done
		}

		// whitespace
		Glyph* glyph = new Glyph{
			' ',
			0.0f,
			0.3f,
			sprite_packer->empty_sprite()
		};
		m_glyphs.insert(std::make_pair(' ', glyph));

		// 0
		Glyph* glyph = new Glyph{
			' ',
			1.0f,
			1.0f,
			sprite_packer->empty_sprite()
		};
		m_glyphs.insert(std::make_pair(0, glyph));

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	Font::~Font() {
		for (auto& g : m_glyphs) {
			if (g.second) delete g.second;
		}
	}

}