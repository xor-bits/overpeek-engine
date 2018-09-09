#include "triangle.h"

namespace graphics {

	GLuint Triangle::VAO;
	GLuint Triangle::VBO;
	GLuint Triangle::IBO;
	bool Triangle::first = false;

	Triangle::Triangle(Shader *shader, glm::vec3 points[], glm::vec4 color) {
		mShader = shader; mColor = color;
		mPoints[0] = points[0];
		mPoints[1] = points[1];
		mPoints[2] = points[2];
		if (!first) { init(); first = true; }
	}

	void Triangle::init() {
		//Generate VAO and bind
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Generate VBO and bind
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, NULL, GL_DYNAMIC_DRAW);

		//Generate IBO and bind
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

		//Attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Triangle::render() {
		mShader->enable();
		mShader->setUniform4f("color", mColor);

		GLfloat vertices[] = {
			mPoints[0].x, mPoints[0].y, mPoints[0].z,
			mPoints[1].x, mPoints[1].y, mPoints[1].z,
			mPoints[2].x, mPoints[2].y, mPoints[2].z
		};

		GLushort indices[]{
			0, 1, 2
		};

		//Bind
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

		//Unbind
		glBindVertexArray(0);
	}

}