#pragma once

#include <string>
#include <map>

#include "engine/graphics/spritePacker.h"
#include "engine/internal_libs.h"

#ifdef WIN32
#define __FONT_NAME		"Arial.ttf"
#define DEFAULT_FONT	"C:/Windows/Fonts/" __FONT_NAME
#elif __linux__
#define __FONT_NAME		"LiberationSans-Regular.ttf"
#define DEFAULT_FONT	"/usr/share/fonts/truetype/liberation/" __FONT_NAME
#else
#define __FONT_NAME		"LiberationSans-Regular.ttf"
#define DEFAULT_FONT	 __FONT_NAME
#endif



namespace oe::graphics {

	class Font {
	public:
		struct Glyph {
			unsigned char character;
			glm::vec2 size;
			glm::vec2 top_left;
			glm::vec2 advance;

			const Sprite* sprite;
		};

	public:
		std::unordered_map<char, Glyph*> m_glyphs;
		int m_resolution;

		float bb_max_height;
		float bb_min_height;
		float bb_height;
	
	public:
		/*
		font_path = "default" loads (Arial.ttf/LiberationSans-Regular.ttf) from
		linux: /usr/share/fonts/truetype/liberation/
		windows: C:/Windows/Fonts/
		macos: *workdir*
		*/
		Font(oe::graphics::SpritePack* sprite_packer, int resolution = 64, std::string font_path = DEFAULT_FONT);
		~Font();

		const Glyph* getGlyph(char c) const { if (m_glyphs.find(c) != m_glyphs.end()) return m_glyphs.at(c); else return nullptr; }
	};

}

