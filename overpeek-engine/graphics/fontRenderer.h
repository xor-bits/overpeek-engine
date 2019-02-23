#pragma once

#include <map>
#include <glm/glm.hpp>

#define TEXT_MAX_QUADS_PER_FLUSH 64000
#define TEXT_VERTEX_PER_QUAD 6 * 2
#define TEXT_MAX_VBO TEXT_MAX_QUADS_PER_FLUSH * TEXT_VERTEX_PER_QUAD

#define FONT_RESOLUTION 128.0
#define FONT_BACKGROUND	8

namespace oe {

	class Renderer;
	class QuadRenderer;

	class FontRenderer {
	private:
		static glm::vec4 colors[9];

		unsigned int texture;
		struct Character {
			unsigned int textureID;		//Glyph texture id
			glm::ivec2 size;				//Glyph size
			glm::ivec2 bearing;			//Offset from baseline to top left
			signed long advance;	//Offset to advance to next glyph
		};
		std::map<char, Character*> m_characters;
		bool initalized = false;
		QuadRenderer *m_renderer;

	private:

		bool init(const char *fontPath);

	public:
		FontRenderer(const char *fontPath, QuadRenderer *renderer);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit text to render
		//_textOrigin = enum textOrigin
		void renderText(float x, float y, float w, float h, const char *text, int _textOrigin);
		
		//Draws buffers and then clears them
		void draw();
	};

}