#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/spritePacker.hpp"



namespace oe::graphics {

	bool Font::gen_codepoint_glyph(char32_t codepoint)
	{
		//Load glyph
		if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
			spdlog::warn("Failed to load glyph: {}", (size_t)codepoint);
			return false;
		}

		// glyph
		auto g = face->glyph;
		if (!g->bitmap.buffer) return false;

		// white font
		uint8_t* data = new uint8_t[g->bitmap.width * g->bitmap.rows * 4]();
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
		Font::Glyph* glyph = new Font::Glyph{
			codepoint,
			glm::vec2(g->bitmap.width, g->bitmap.rows) / (float)m_resolution,
			glm::vec2(g->bitmap_left, -g->bitmap_top) / (float)m_resolution,
			glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution,

			// add glyph to sprite packer
			m_sprite_pack->create(std::move(oe::utils::image_data(data, oe::formats::rgba, g->bitmap.width, g->bitmap.rows)))
		};
		m_glyphs.insert(std::make_pair(codepoint, glyph));

		FT_Outline_Embolden;

		delete[] data;
		return true;
	}

	Font::Font(uint16_t resolution, const std::string& font_path)
		: m_resolution(resolution)
		, m_sprite_pack(new SpritePack(5))
	{
		oe_debug_call("font");

		// Freetype library
		if (FT_Init_FreeType(&ft)) {
			oe_error_terminate("FT_Init_FreeType failed");
			return;
		}

		// Font
		if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
			oe_error_terminate("Failed to load font at path {}", font_path.c_str());
			return;
		}
		
		FT_Set_Pixel_Sizes(face, 0, resolution);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		// all ascii glyphs
		for (char32_t i = 32; i < 127; i++) {
			gen_codepoint_glyph(i);
		}

		// whitespace
		Glyph* glyph = new Glyph{
			L' ',
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.3f),
			m_sprite_pack->emptySprite()
		};
		m_glyphs.insert(std::make_pair(' ', glyph));

		m_sprite_pack->constructRepeat();
	}

	Font::~Font() {
		for (auto& g : m_glyphs) {
			if (g.second) delete g.second;
		}
		delete m_sprite_pack;

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
	
	const Font::Glyph* Font::getGlyph(char32_t c)
	{
		if (m_glyphs.find(c) != m_glyphs.end()) 
		{
			// glyph found
			return m_glyphs.at(c); 
		}
		else
		{
			// glyph not found, try generate it
			bool successful = gen_codepoint_glyph(c);
			if (!successful) return nullptr;
			
			m_sprite_pack->constructRepeat(); // frametime jumps when new character is seen
			if (m_glyphs.find(c) != m_glyphs.end()) 
			{
				// glyph found
				return m_glyphs.at(c); 
			}
			else 
			{
				return nullptr;
			}
		}
		return nullptr;
	}

}