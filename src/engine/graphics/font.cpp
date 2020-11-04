#include "font.hpp"

// #define OE_USE_FT2
#ifdef OE_USE_FT2
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>
#else
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#endif

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/formatted_error.hpp"



namespace oe::graphics
{
	struct FontData
	{
#ifdef OE_USE_FT2
		FT_LibraryRec_ ft;
		FT_FaceRec_ face;
#else
		stbtt_fontinfo info;
		float scale;
#endif
	};



	bool Font::gen_codepoint_glyph(char32_t codepoint)
	{
#ifdef OE_USE_FT2
		//Load glyph
		if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER))
		{
			spdlog::warn("Failed to load glyph: {}", (size_t)codepoint);
			return false;
		}

		// glyph
		auto g = face->glyph;
		if (!g->bitmap.buffer) return false;

		// white font
		uint8_t* data = new uint8_t[g->bitmap.width * g->bitmap.rows * 4]();
		for (size_t y = 0; y < g->bitmap.rows; y++) {
			for (size_t x = 0; x < g->bitmap.width; x++) {
				size_t pixel = (x + y * g->bitmap.width) * 4;

				data[pixel + 0] = 255;
				data[pixel + 1] = 255;
				data[pixel + 2] = 255;
				data[pixel + 3] = g->bitmap.buffer[x + y * g->bitmap.width];
			}
		}

		//Now store character for later use
		m_glyphs.insert(std::make_pair(codepoint, Font::Glyph{
			codepoint,
			glm::vec2(g->bitmap.width, g->bitmap.rows) / (float)m_resolution,
			glm::vec2(g->bitmap_left, -g->bitmap_top) / (float)m_resolution,
			glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution,

			// add glyph to sprite packer
			m_sprite_pack->create(std::move(oe::utils::image_data(data, oe::formats::rgba, g->bitmap.width, g->bitmap.rows)))
		}));

		delete[] data;
#else
		// glyph info
		int x0, y0, x1, y1;
		stbtt_GetCodepointBitmapBox(&m_data->info, codepoint, m_data->scale, m_data->scale, &x0, &y0, &x1, &y1);
		int w = std::abs(x0 - x1);
		int h = std::abs(y0 - y1);

		// allocate glyph img
		oe::utils::image_data id(oe::formats::mono, w, h);
		
		// gen glyph img
		stbtt_MakeCodepointBitmap(&m_data->info, id.data, w, h, 0, m_data->scale, m_data->scale, codepoint);
		
		//Now store character for later use
		m_glyphs.insert(std::make_pair(codepoint, Font::Glyph{
			codepoint,
			glm::vec2(w, h) / (float)m_resolution,
			glm::vec2(0, 0) / (float)m_resolution,
			glm::vec2(w, h) / (float)m_resolution,

			// add glyph to sprite packer
			m_sprite_pack->create(std::move(id))
		}));
#endif

		return true;
	}
	
	Font::Font(uint16_t resolution, const oe::utils::FontFile& font_file)
		: m_sprite_pack(new SpritePack(5))
		, m_resolution(resolution)
		, m_data(new FontData())
		, m_font_file(font_file.fontFile())
	{
		oe_debug_call("font");

		if (!stbtt_InitFont(&m_data->info, m_font_file.data(), 0))
			throw oe::utils::formatted_error("Failed to load font in memory {0:x} {1}", (size_t)m_font_file.data(), m_font_file.size());

    	m_data->scale = stbtt_ScaleForPixelHeight(&m_data->info, m_resolution);

		/* // Freetype library
		if (FT_Init_FreeType(&ft))
			throw oe::utils::formatted_error("FT_Init_FreeType failed");

		// Load font
		if (FT_New_Memory_Face(ft, m_font_file.data(), static_cast<int32_t>(std::clamp<size_t>(m_font_file.size(), 0, std::numeric_limits<int32_t>::max())), 0, &face))
			throw oe::utils::formatted_error("Failed to load font in memory {0:x} {1}", (size_t)m_font_file.data(), m_font_file.size());

		FT_Set_Pixel_Sizes(face, 0, m_resolution);
		FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction */

		// all ascii glyphs
		for (char32_t i = 32; i < 127; i++)
			gen_codepoint_glyph(i);

		// whitespace
		m_glyphs.insert(std::make_pair(' ', Glyph{
			L' ',
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.3f),
			m_sprite_pack->emptySprite()
		}));

		m_sprite_pack->constructRepeat();
	}

	Font::~Font() {
		delete m_sprite_pack;
		delete m_data;

#ifdef OE_USE_FT2
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
#else
		// 
#endif
	}
	
	const Font::Glyph* Font::getGlyph(char32_t c)
	{
		if (m_glyphs.find(c) != m_glyphs.end()) 
		{
			// glyph found
			return &m_glyphs.at(c); 
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
				return &m_glyphs.at(c); 
			}
			else 
			{
				return nullptr;
			}
		}
		return nullptr;
	}

}