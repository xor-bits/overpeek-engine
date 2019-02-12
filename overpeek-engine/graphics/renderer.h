#pragma once

#include <string>
#include <glm/glm.hpp>

namespace oe {

	enum textOrigin
	{
		topLeft, topCenter, topRight, centerLeft, center, centerRight, bottomLeft, bottomCenter, bottomRight
	};


	class FontRenderer;
	class QuadRenderer;
	class PointRenderer;
	class Window;
	class VertexArray;
	class Buffer;
	class Shader;

	class Renderer {
	private:
		//Font
		FontRenderer *m_fontRenderer;

		//Quad
		QuadRenderer *m_quadRenderer;

		//Point
		PointRenderer *m_pointRenderer;

		//Dependecies
		Window *m_window;


		//Framebuffers
		unsigned int m_framebuffer1;
		unsigned int m_frametexture1;
		unsigned int m_framebuffer2;
		unsigned int m_frametexture2;
		VertexArray *m_ScreenVAO;
		Buffer *m_ScreenVBO;

		bool m_quadMapped;
		bool m_fontMapped;
		bool m_pointMapped;

	public:
		Renderer(std::string font, Window *window);

		~Renderer();

		//Submit quad to renderer
		void renderBox(glm::vec2 _pos, glm::vec2 _size, int _id, glm::vec4 _color);

		//Submit point to renderer
		void renderPoint(glm::vec2 pos, glm::vec2 size, int id, glm::vec4 color);

		//Submit text to renderer
		void renderText(glm::vec2 pos, glm::vec2 size, std::string text, glm::vec3 color, int _textOrigin);

		//Draws all quads and text
		//textbool is location of int (used as boolean) in shader that enables or disables text rendering
		void draw(Shader *shader, Shader * pointshader, std::string textbool, int texture);

		//Draws all quads and text to specified framebuffer at index
		void drawToFramebuffer(Shader *shader, Shader * pointshader, std::string textbool, int texture, bool first_framebuffer);
		
		//Draws (first or second) framebuffer to screen
		void drawFramebuffer(Shader *postshader, std::string texture_sampler, bool first_framebuffer);
		
		//Draws framebuffer to another framebuffer
		void drawFramebufferToFramebuffer(Shader *postshader, std::string texture_sampler, bool first_framebuffer);

	};

}