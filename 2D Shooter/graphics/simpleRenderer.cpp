#include "simpleRenderer.h"


namespace graphics {

	GLuint SimpleRenderer::mTriangleVAO;
	GLuint SimpleRenderer::mTriangleVBO;
	GLuint SimpleRenderer::mTriangleIBO;

	void SimpleRenderer::init() {
		GLfloat vertices[3][2] = {
			{  0.0f, -1.0f },
			{  1.0f,  1.0f },
			{ -1.0f,  1.0f }
		};
		setArrayObjects(vertices, mTriangleVAO, mTriangleVBO, mTriangleIBO);
	}

	void SimpleRenderer::setArrayObjects(GLfloat vertices[][2], GLuint &vao, GLuint &vbo, GLuint &ibo) {
		//Generate VAO and bind
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Generate VBO and bind
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Set vertexdata
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 2, vertices, GL_STATIC_DRAW);

		//Attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SimpleRenderer::renderTriangle(Shader *shader, glm::mat4 *ml_matrix, glm::vec4 color) {
		if (ml_matrix == NULL) ml_matrix = new glm::mat4(1.0f);

		shader->enable();
		shader->setUniform4f("color", color);
		shader->SetUniformMat4("ml_matrix", *ml_matrix);
		delete ml_matrix;

		//Bind
		glBindVertexArray(mTriangleVAO);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind
		glBindVertexArray(0);
	}

	//void SimpleRenderer::renderCircle(glm::mat4 modelMatrix, float x, float y, float r, unsigned int resolution, glm::vec4 color) {
	//
	//	std::vector<glm::vec2> points;
	//	points.push_back(glm::vec2(x, y));
	//	points.push_back(glm::vec2(x, y));
	//	points.push_back(glm::vec2(x, y));
	//
	//	float pi2ByResolution = glm::two_pi<float>() / resolution;
	//
	//	for (int i = 0; i < resolution; i++) {
	//		points[1] = glm::vec2(r * sin(pi2ByResolution * i), r * cos(pi2ByResolution * i));
	//		points[2] = glm::vec2(r * sin(pi2ByResolution * (i + 1)), r * cos(pi2ByResolution * (i+1)));
	//		//renderTriangle(modelMatrix, 0.0f, 0.0f, points, color);
	//	}
	//}

}