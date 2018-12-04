#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"
#include "fontLoader.h"
#include "buffers/indexBuffer.h"
#include "buffers/buffer.h"
#include "buffers/vertexArray.h"

#define MAX_QUADS_PER_FLUSH 64000
#define VERTEX_PER_QUAD 4 * 2
#define INDEX_PER_QUAD 6
#define MAX_VBO MAX_QUADS_PER_FLUSH * VERTEX_PER_QUAD
#define MAX_IBO MAX_QUADS_PER_FLUSH * INDEX_PER_QUAD

namespace graphics {

	class Renderer {
	private:
		VertexArray *m_VAO;
		Buffer *m_VBO;
		Buffer *m_UV;
		Buffer *m_ID;

		IndexBuffer *m_IBO;

		GLuint quadCount;
		GLfloat m_vertex[MAX_VBO];
		GLfloat m_uv[MAX_VBO];
		GLfloat m_id[MAX_VBO];
		GLushort m_index[MAX_IBO];

		FontLoader *fontLoader;

		void initText(std::string fontpath);

		bool textRender = false;

	public:
		Renderer(std::string fontpath);
		Renderer();

		void renderBox(float x, float y, float w, float h, float angle, int textureID);
		void renderBoxCentered(float x, float y, float w, float h, float angle, int textureID);
		void renderText(float x, float y, float w, float h, float angle, std::string text, glm::vec3 color, int xAlign, int yAlign);
		
		void flush(Shader *shader, int quadTexture);
		void clear();
	};
	
}