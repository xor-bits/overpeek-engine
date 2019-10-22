#pragma once

#include <string>
#include <map>

#include "../texture.h"
#include "../../internal_libs.h"



namespace oe {

	class Font {
	public:
		struct Glyph {
			unsigned char character;
			glm::vec2 size;
			glm::vec2 top_left;
			glm::vec2 advance;
		};

	public:
		Texture m_glyph_texture;
		Glyph* m_glyphs[256];
		int m_resolution;
	
	public:
		Font(int resolution = 64, std::string font_path = "calibri.ttf");
		~Font();

		inline void bindTexture() const { m_glyph_texture.bind(); }
	};

}

