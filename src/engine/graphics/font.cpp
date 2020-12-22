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

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

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
		stbtt_fontinfo info;
		float scale;
	};



	bool Font::gen_codepoint_glyph(char32_t codepoint)
	{
		if(m_sdf)
		{
			// glyph info
			int x0, y0, w, h;
			uint8_t* data = stbtt_GetCodepointSDF(&m_data->info, m_data->scale, codepoint, 12, 192, 16.0f, &w, &h, &x0, &y0);
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
			stbtt_FreeSDF(data, nullptr);
			
			// Now store character for later use
			m_glyphs.try_emplace(codepoint,
				codepoint,
				glm::vec2{ w, h }                             / static_cast<float>(m_resolution),
				glm::vec2{ x0, y0 }                           / static_cast<float>(m_resolution),
				static_cast<float>(advance_x) * m_data->scale / static_cast<float>(m_resolution),

				// add glyph to sprite packer
				m_sprite_pack->create(std::move(id))
			);
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

			// Now store character for later use
			m_glyphs.try_emplace(codepoint,
				codepoint,
				glm::vec2{ w, h }                             / static_cast<float>(m_resolution),
				glm::vec2{ x0, y0 }                           / static_cast<float>(m_resolution),
				static_cast<float>(advance_x) * m_data->scale / static_cast<float>(m_resolution),

				// add glyph to sprite packer
				m_sprite_pack->create(std::move(id))
			);
		}

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
		: m_sprite_pack(new SpritePack(5))
		, m_data(new FontData())
		, m_resolution(resolution)
		, m_sdf(sdf)
		, m_font_file(font_file.fontFile())
	{
		if (!stbtt_InitFont(&m_data->info, m_font_file.data(), 0))
			throw oe::utils::formatted_error("Failed to load font in memory {0:x} {1}", (size_t)m_font_file.data(), m_font_file.size());

    	m_data->scale = stbtt_ScaleForPixelHeight(&m_data->info, m_resolution);

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&m_data->info, &ascent, &descent, &line_gap);
		m_ascent = ascent * m_data->scale / static_cast<float>(m_resolution);
		m_descent = descent * m_data->scale / static_cast<float>(m_resolution);
		m_line_gap = line_gap * m_data->scale / static_cast<float>(m_resolution);
		m_height = std::abs(m_ascent - m_descent);

		// all ascii glyphs
		for (char32_t i = 33; i < 127; i++)
			gen_codepoint_glyph(i);

		// null
		m_glyphs.try_emplace(U'\0',
			U'\0',
			glm::vec2{ 0.0f, 0.0f },
			glm::vec2{ 0.0f, 0.0f },
			0.0f,
			m_sprite_pack->emptySprite()
		);

		// whitespace
		m_glyphs.try_emplace(U' ',
			U' ',
			glm::vec2{ 0.0f, 0.0f },
			glm::vec2{ 0.0f, 0.0f },
			0.3f,
			m_sprite_pack->emptySprite()
		);

		m_sprite_pack->constructRepeat(default_texture_settings(m_sdf));
	}

	Font::~Font()
	{
		delete m_sprite_pack;
		delete m_data;
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