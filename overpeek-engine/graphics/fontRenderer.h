#pragma once

#include <map>
#include <glm/glm.hpp>

#define TEXT_MAX_QUADS_PER_FLUSH 64000
#define TEXT_VERTEX_PER_QUAD 6 * 2
#define TEXT_MAX_VBO TEXT_MAX_QUADS_PER_FLUSH * TEXT_VERTEX_PER_QUAD

#define FONT_RESOLUTION		512.0f
#define FONT_BG_EXTEND_X	7.0f / 8.0f
#define FONT_BG_EXTEND_Y	7.0f / 8.0f
//#define FONT_BACKGROUND	FONT_RESOLUTION / 8

namespace oe {

	class Renderer;
	class QuadRenderer;

	class FontRenderer {
	private:
		static glm::vec4 colors[9];

		unsigned int texture;
		struct Character {
			unsigned int textureID;		//Glyph texture id
			glm::ivec2 size;	    	//Glyph size
			glm::ivec2 bearing;			//Offset from baseline to top left
			signed long advance;	    //Offset to advance to next glyph
		};
		std::map<char, Character*> m_characters;
		bool initalized = false;
		QuadRenderer *m_renderer;
		Renderer* m_ptr_to_parent;

		void getStartingPos(glm::vec3& pos, float& final_width, glm::vec2 scale, const char* _text, int _textOrigin);

	private:

		bool init(const char *fontPath);

	public:
		FontRenderer(const char *fontPath, Renderer *parent);

		//Maps buffer
		void beginRendering();

		//Unmaps buffer
		void stopRendering();

		///TODO:
		///BAKED TEXT RENDERING

		//Submit text to render
		//_textOrigin = enum textOrigin
		void renderText(glm::vec3 pos, glm::vec2 scale, const char* text);

		//Submit text to render
		//_textOrigin = enum textOrigin
		void renderText(glm::vec3 pos, glm::vec2 scale, const char* text, int _textOrigin);
		
		//Submit text to render
		//_textOrigin = enum textOrigin
		void renderText(glm::vec3 pos, glm::vec2 scale, const char* text, int _textOrigin, glm::vec4 background);
		
		//Draws buffers and then clears them
		void draw();
	};

}