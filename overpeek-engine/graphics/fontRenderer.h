#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#define TEXT_MAX_QUADS_PER_FLUSH 64000
#define TEXT_VERTEX_PER_QUAD 6 * 2
#define TEXT_MAX_VBO TEXT_MAX_QUADS_PER_FLUSH * TEXT_VERTEX_PER_QUAD

namespace oe {

	class Renderer;
	class QuadRenderer;

	class FontRenderer {
	private:
		GLuint texture;
		struct Character {
			GLuint textureID;		//Glyph texture id
			glm::ivec2 size;	//Glyph size
			glm::ivec2 bearing; //Offset from baseline to top left
			FT_Pos advance;		//Offset to advance to next glyph
		};
		std::map<GLchar, Character*> m_characters;
		bool initalized = false;
		QuadRenderer *m_renderer;

	private:

		bool init(std::string &fontPath);

	public:
		FontRenderer(std::string &fontPath, QuadRenderer *renderer);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		//Submit text to render
		//_textOrigin = enum textOrigin
		void renderText(float x, float y, float w, float h, std::string &text, glm::vec3 color, int _textOrigin);
		
		//Draws buffers and then clears them
		void draw();
	};

}