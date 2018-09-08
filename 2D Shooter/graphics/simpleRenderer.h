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
		GLuint mTriangleVAO, mTriangleVBO, mTriangleIBO;
		Shader *mShader;

	public:
		SimpleRenderer(Shader &shader);

		void setArrayObjects(GLfloat vertices[][2], GLuint &vao, GLuint &vbo, GLuint &ibo);

		void renderTriangle(glm::mat4 modelMatrix, glm::vec4 color);
	};
	
}