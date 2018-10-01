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

namespace graphics {

	class SimpleRenderer {
	private:
		static VertexArray *m_VAO;
		static Buffer *m_VBO;
		static Buffer *m_TBO;
		static IndexBuffer *m_IBO;
		
		static FontLoader *m_fontLoader;
		static Shader *m_shader;
		static Shader *m_textureshader;
		static Shader *m_textShader;

		static glm::vec4 m_color;
		
		static bool renderInitialized;
		static bool fontInitialized;

	public:
		static void init(Shader *shader, Shader *textureshader, Shader *textShader, std::string fontpath);

		static void renderBox(float x, float y, float w, float h);
		static void renderBox(float x, float y, float w, float h, GLuint texture);
		static void renderText(float x, float y, float w, float h, std::string text, glm::vec3 color, int xAlign, int yAlign);
		
		static void setColor(glm::vec4 color) { m_color = color; }

		static inline void setShader(Shader *shader) { m_shader = shader; }
		static inline Shader *getShader() { return m_shader; }
	};
	
}