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
		GLuint mVAOQuad, mVBOQuad;
		GLuint mVAOTri, mVBOTri;
		GLuint mVAOLn, mVBOLn;
		Shader *mShader;

	public:
		SimpleRenderer(Shader &shader);

		void renderQuad(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color);
		void renderQuad(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color);

		void renderTriangle(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color);
		void renderTriangle(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color);
		
		void renderCircle(glm::mat4 modelMatrix, float x, float y, float r, unsigned int resolution, glm::vec4 color);
		void renderLine(glm::mat4 modelMatrix, float w, glm::vec2 A, glm::vec2 B, glm::vec4 color);
	};
	
}