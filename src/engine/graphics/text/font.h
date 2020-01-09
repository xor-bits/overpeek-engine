#pragma once

#include <string>
#include <map>

#include "engine/graphics/texture.h"
#include "engine/internal_libs.h"

#define CHAR_COUNT 128

#ifdef WIN32
#define DEFAULT_FONT	"C:/Windows/Fonts/LiberationSans-Regular.ttf"
#elif __linux__
#define DEFAULT_FONT	"/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
#else
#define DEFAULT_FONT	"LiberationSans-Regular.ttf"
#endif



namespace oe::graphics {

	class Font {
	public:
		struct Glyph {
			unsigned char character;
			glm::vec2 size;
			glm::vec2 top_left;
			glm::vec2 advance;
		};

		void* m_ft;
		void* m_face;
		unsigned char* m_data;

	public:
		Texture m_glyph_texture;
		Glyph* m_glyphs[256];
		int m_resolution;

		float bb_max_height;
		float bb_min_height;
		float bb_height;
	
	public:
		/*
		font_path = "default" loads LiberationSans-Regular.ttf from
		linux: /usr/share/fonts/truetype/liberation/
		windows: C:/Windows/Fonts/
		macos: *workdir*
		*/
		Font(int resolution = 64, bool manual_finish = false, std::string font_path = DEFAULT_FONT);
		~Font();

		// Must be executed on main thread (OpenGL thread)
		void finish();

		inline void bindTexture() const { m_glyph_texture.bind(); }
	};

}

