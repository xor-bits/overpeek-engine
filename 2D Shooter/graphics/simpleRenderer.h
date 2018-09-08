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
<<<<<<< HEAD
		GLuint mVAOQuad, mVBOQuad, mVAOTri, mVBOTri, mVAOPoint, mVBOPoint;
=======
		GLuint mVAOQuad, mVBOQuad;
		GLuint mVAOTri, mVBOTri;
		GLuint mVAOLn, mVBOLn;
>>>>>>> 64784bb13d14e080ca926845f21fc4c6be53f0f4
		Shader *mShader;

	public:
		SimpleRenderer(Shader &shader);

<<<<<<< HEAD
		void renderPoints(std::vector<glm::vec2> points, glm::vec4 color);

		void renderQuad(float x, float y, float w, float h, glm::vec4 color);
		void renderQuad(float x, float y, std::vector<glm::vec2> points, glm::vec4 color);

		void renderTriangle(float x, float y, float w, float h, glm::vec4 color);
		void renderTriangle(float x, float y, std::vector<glm::vec2> points, glm::vec4 color);
=======
		void renderQuad(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color);
		void renderQuad(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color);
>>>>>>> 64784bb13d14e080ca926845f21fc4c6be53f0f4

		void renderTriangle(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color);
		void renderTriangle(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color);
		
		void renderCircle(glm::mat4 modelMatrix, float x, float y, float r, unsigned int resolution, glm::vec4 color);
		void renderLine(glm::mat4 modelMatrix, float w, glm::vec2 A, glm::vec2 B, glm::vec4 color);
	};
	
}