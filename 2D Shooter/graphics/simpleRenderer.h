#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"


namespace graphics {

	class SimpleRenderer {
	private:
		GLuint mVAOQuad, mVBOQuad, mVAOTri, mVBOTri;

	public:
		SimpleRenderer();

		void renderQuad(Shader &shader, float x, float y, float w, float h, glm::vec4 color);
		void renderQuad(Shader &shader, glm::vec2 points[4], glm::vec4 color);

		void renderTriangle(Shader &shader, float x, float y, float w, float h, glm::vec4 color);
		void renderTriangle(Shader &shader, glm::vec2 points[3], glm::vec4 color);

	};

}