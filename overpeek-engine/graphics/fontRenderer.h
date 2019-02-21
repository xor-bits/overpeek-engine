#pragma once

#include <map>
#include <glm/glm.hpp>

#define TEXT_MAX_QUADS_PER_FLUSH 64000
#define TEXT_VERTEX_PER_QUAD 6 * 2
#define TEXT_MAX_VBO TEXT_MAX_QUADS_PER_FLUSH * TEXT_VERTEX_PER_QUAD

#define FONT_RESOLUTION 48.0
#define FONT_BORDER		128
#define TEXTURE_SCALAR	1.2

namespace oe {

	class Renderer;
	class QuadRenderer;

	class FontRenderer {
	private:
		unsigned int texture;
		struct Character {
			unsigned int textureID;
			glm::ivec2 size;
			glm::ivec2 bearing;
			signed long advance;
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
		void renderText(float x, float y, float w, float h, const char *text, glm::vec3 color, int _textOrigin);
		
		//Draws buffers and then clears them
		void draw();
	};

}