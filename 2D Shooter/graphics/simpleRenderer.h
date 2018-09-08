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
		static GLuint mTriangleVAO, mTriangleVBO, mTriangleIBO;

		static void setArrayObjects(GLfloat vertices[][2], GLuint &vao, GLuint &vbo, GLuint &ibo);

	public:
		static void init();

		static void renderTriangle(Shader *shader, glm::mat4 *ml_matrix, glm::vec4 color);

	};
	
}