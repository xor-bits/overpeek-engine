#include "font.hpp"

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"
#include "engine/graphics/spritePacker.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/formatted_error.hpp"



// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

// #define OE_USE_FT2 // does not use FT2 by default
#ifdef OE_USE_FT2
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftstroke.h>
#else
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#endif

// ignore external warnings
#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif



namespace oe::graphics
{
	struct FontData
	{
#ifdef OE_USE_FT2
		FT_Library ft;
		FT_Face face;
#else
		stbtt_fontinfo info;
		float scale;
#endif
	};



	bool Font::gen_codepoint_glyph(char32_t codepoint)
	{
#ifdef OE_USE_FT2
		//Load glyph
		if (FT_Load_Char(m_data->face, codepoint, FT_LOAD_RENDER))
		{
			spdlog::warn("Failed to load glyph: {}", (size_t)codepoint);
			return false;
		}

		// glyph
		const auto g = m_data->face->glyph;
		if (!g->bitmap.buffer) return false;

		//Now store character for later use
		m_glyphs.insert(std::make_pair(codepoint, Font::Glyph{
			codepoint,
			glm::vec2(g->bitmap.width, g->bitmap.rows) / (float)m_resolution,
			glm::vec2(g->bitmap_left, -g->bitmap_top) / (float)m_resolution,
			glm::vec2(g->advance.x >> 6, g->advance.y >> 6) / (float)m_resolution,

			// add glyph to sprite packer
			m_sprite_pack->create(std::move(oe::utils::image_data(g->bitmap.buffer, oe::formats::mono, g->bitmap.width, g->bitmap.rows)))
		}));

		m_topmost = std::min(m_topmost, static_cast<float>(-g->bitmap_top) / static_cast<float>(m_resolution));
#else
		if(m_sdf)
		{
			// glyph info
			int x0, y0, w, h;
			uint8_t* data = stbtt_GetCodepointSDF(&m_data->info, m_data->scale, codepoint, 8, 192, 16.0f, &w, &h, &x0, &y0);
			int advance_x, bearing_x;
			stbtt_GetCodepointHMetrics(&m_data->info, codepoint, &advance_x, &bearing_x);
			int advance_y, bearing_y;
			stbtt_GetCodepointHMetrics(&m_data->info, codepoint, &advance_y, &bearing_y);

			if(!data) {
				spdlog::warn("Could not load codepoint {:x}", (uint32_t)codepoint);
				return false;
			}

			// glyph img
			oe::utils::image_data id(data, oe::formats::mono, w, h);
			
			//Now store character for later use
			m_glyphs.insert(std::make_pair(codepoint, Font::Glyph{
				codepoint,
				glm::vec2(w, h) / (float)m_resolution,
				glm::vec2(x0, y0) / (float)m_resolution,
				glm::vec2(advance_x, advance_y) * m_data->scale / (float)m_resolution,

				// add glyph to sprite packer
				m_sprite_pack->create(std::move(id))
			}));

			m_topmost = std::min(m_topmost, static_cast<float>(y0) / static_cast<float>(m_resolution));
		}
		else
		{
			// glyph info
			int x0, y0, x1, y1;
			stbtt_GetCodepointBitmapBox(&m_data->info, codepoint, m_data->scale, m_data->scale, &x0, &y0, &x1, &y1);
			int advance_x, bearing_x;
			stbtt_GetCodepointHMetrics(&m_data->info, codepoint, &advance_x, &bearing_x);
			int advance_y, bearing_y;
			stbtt_GetCodepointHMetrics(&m_data->info, codepoint, &advance_y, &bearing_y);
			const int w = std::abs(x0 - x1);
			const int h = std::abs(y0 - y1);

			// allocate glyph img
			oe::utils::image_data id(oe::formats::mono, w, h);
			
			// gen glyph img
			stbtt_MakeCodepointBitmap(&m_data->info, id.data, w, h, w, m_data->scale, m_data->scale, codepoint);

			//Now store character for later use
			m_glyphs.insert(std::make_pair(codepoint, Font::Glyph{
				codepoint,
				glm::vec2(w, h) / (float)m_resolution,
				glm::vec2(x0, y0) / (float)m_resolution,
				glm::vec2(advance_x, advance_y) * m_data->scale / (float)m_resolution,

				// add glyph to sprite packer
				m_sprite_pack->create(std::move(id))
			}));

			m_topmost = std::min(m_topmost, static_cast<float>(y0) / static_cast<float>(m_resolution));
		}
#endif

		return true;
	}

	[[nodiscard]] inline static oe::TextureInfo default_texture_settings(bool sdf) noexcept
	{
		oe::TextureInfo info;
		info.generate_mipmaps = !sdf;
		info.filter = sdf ? oe::texture_filter::linear : oe::texture_filter::nearest;
		return info;
	}
	
	Font::Font(uint16_t resolution, bool sdf, const oe::utils::FontFile& font_file)
		: m_topmost(0.0f)
		, m_sprite_pack(new SpritePack(5))
		, m_data(new FontData())
		, m_resolution(resolution)
		, m_sdf(sdf)
		, m_font_file(font_file.fontFile())
	{
		oe_debug_call("font");

#ifdef OE_USE_FT2
		// Freetype library
		if (FT_Init_FreeType(&m_data->ft))
			throw oe::utils::formatted_error("FT_Init_FreeType failed");

		// Load font
		if (FT_New_Memory_Face(m_data->ft, m_font_file.data(), static_cast<int32_t>(std::clamp<size_t>(m_font_file.size(), 0, std::numeric_limits<int32_t>::max())), 0, &m_data->face))
			throw oe::utils::formatted_error("Failed to load font in memory {0:x} {1}", (size_t)m_font_file.data(), m_font_file.size());

		FT_Set_Pixel_Sizes(m_data->face, 0, m_resolution);
		FT_Select_Charmap(m_data->face, FT_ENCODING_UNICODE);
#else
		if (!stbtt_InitFont(&m_data->info, m_font_file.data(), 0))
			throw oe::utils::formatted_error("Failed to load font in memory {0:x} {1}", (size_t)m_font_file.data(), m_font_file.size());

    	m_data->scale = stbtt_ScaleForPixelHeight(&m_data->info, m_resolution);
#endif

		// all ascii glyphs
		for (char32_t i = 33; i < 127; i++)
			gen_codepoint_glyph(i);

		// null
		m_glyphs.insert(std::make_pair(U'\0', Glyph{
			U'\0',
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			m_sprite_pack->emptySprite()
		}));

		// whitespace
		m_glyphs.insert(std::make_pair(U' ', Glyph{
			U' ',
			glm::vec2(0.0f),
			glm::vec2(0.0f),
			glm::vec2(0.3f),
			m_sprite_pack->emptySprite()
		}));

		m_sprite_pack->constructRepeat(default_texture_settings(m_sdf));
	}

	Font::~Font()
	{
		delete m_sprite_pack;
		delete m_data;

#ifdef OE_USE_FT2
		FT_Done_Face(m_data->face);
		FT_Done_FreeType(m_data->ft);
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
			
			m_sprite_pack->constructRepeat(default_texture_settings(m_sdf)); // frametime jumps when new character is seen
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

	float Font::getKerning(char32_t c, char32_t c_next) const
	{
#ifdef OE_USE_FT2
		return 0.0f;
#else
		return stbtt_GetCodepointKernAdvance(&m_data->info, c, c_next) * m_data->scale / (float)m_resolution;
#endif
	}
}