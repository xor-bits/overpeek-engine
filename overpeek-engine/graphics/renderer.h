#pragma once

#include <glm/glm.hpp>

#include "fontRenderer.h"
#include "quadRenderer.h"
#include "pointRenderer.h"
#include "vertexData.h"
#include "lineRenderer.h"
#include "triangleRenderer.h"


namespace oe {

	enum textOrigin
	{
		topLeft, topCenter, topRight, centerLeft, center, centerRight, bottomLeft, bottomCenter, bottomRight
	};

	class VertexArray;
	class Buffer;
	class Shader;

	class Renderer {
	private:
		//Framebuffers
		unsigned int m_framebuffer1;
		unsigned int m_frametexture1;
		unsigned int m_framebuffer2;
		unsigned int m_frametexture2;
		VertexArray *m_ScreenVAO;
		Buffer *m_ScreenVBO;

	public:
		//Font
		FontRenderer* fontRenderer;

		//Quad
		QuadRenderer* quadRenderer;

		//Line
		LineRenderer* lineRenderer;

		//Point
		PointRenderer* pointRenderer;

		//Triangle
		TriangleRenderer* triangleRenderer;

		//Texture shader
		Shader* shader;

		//Post-process shader
		Shader* post_shader;


	public:
		Renderer(const char *font, int window_width, int window_height);

		~Renderer();

		//Draws all quads and text
		//textbool is location of int (used as boolean) in shader that enables or disables text rendering
		void draw(int texture, bool textureArray);

		//Draws all quads and text to specified framebuffer at index
		void drawToFramebuffer(int texture, bool textureArray, bool first_framebuffer, int window_width, int window_height);
		
		//Draws (first or second) framebuffer to screen
		void drawFramebuffer(const char *texture_sampler, bool first_framebuffer);
		
		//Draws framebuffer to another framebuffer
		void drawFramebufferToFramebuffer(const char *texture_sampler, bool first_framebuffer);
	};

}