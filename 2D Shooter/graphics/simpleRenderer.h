#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"


namespace graphics {

	class SimpleRenderer {
	private:
		GLuint mVAOQuad, mVBOQuad, mVAOTri, mVBOTri;
		Shader *mShader;

	public:
		SimpleRenderer(Shader &shader);

		void renderQuad(float x, float y, float w, float h, glm::vec4 color);
		void renderQuad(float x, float y, std::vector<glm::vec2> points, glm::vec4 color);

		void renderTriangle(float x, float y, float w, float h, glm::vec4 color);
		void renderTriangle(float x, float y, std::vector<glm::vec2> points, glm::vec4 color);

	};

}