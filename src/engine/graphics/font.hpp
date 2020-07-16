#pragma once

#include <string>
#include <map>

#include "engine/graphics/spritePacker.hpp"
#include "engine/internal_libs.hpp"

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


struct FT_LibraryRec_; struct FT_FaceRec_;
namespace oe::graphics {

	class Font {
	public:
		struct Glyph {
			char32_t codepoint;
			glm::vec2 size;
			glm::vec2 top_left;
			glm::vec2 advance;

			const Sprite* sprite;
		};

	private:
		SpritePack m_sprite_pack;
		std::unordered_map<char32_t, Glyph*> m_glyphs;
		int m_resolution;
		FT_LibraryRec_* ft;
		FT_FaceRec_* face;

		bool gen_codepoint_glyph(char32_t codepoint);
	
	public:
		/*
		font_path = "default" loads (Arial.ttf/LiberationSans-Regular.ttf) from
		linux: /usr/share/fonts/truetype/liberation/
		windows: C:/Windows/Fonts/
		macos: *workdir*
		*/
		Font(int resolution = 64, std::string font_path = DEFAULT_FONT);
		~Font();

		const Glyph* getGlyph(char32_t c);
		int getResolution() const { return m_resolution; }
		SpritePack* getSpritePack() { return &m_sprite_pack; }
	};

}

