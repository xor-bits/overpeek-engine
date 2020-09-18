#pragma once

#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"

#include <string>
#include <unordered_map>



struct FT_LibraryRec_; struct FT_FaceRec_;
namespace oe::graphics { struct Sprite; class SpritePack; }
namespace oe::utils { using byte_string = std::vector<uint8_t>; };

namespace oe::graphics
{
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
		SpritePack* m_sprite_pack;
		std::unordered_map<char32_t, Glyph*> m_glyphs;
		uint16_t m_resolution;
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
		Font(const oe::utils::byte_string& font_file, uint16_t resolution = 64u);
		Font(uint16_t resolution = 64u, const std::string& font_path = oe::default_full_font_path);
		~Font();

		const Glyph* getGlyph(char32_t c);
		uint16_t getResolution() const { return m_resolution; }
		SpritePack* getSpritePack() { return m_sprite_pack; }
	};

}

