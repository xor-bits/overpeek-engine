#include "quad.h"

namespace graphics {

	GLuint Quad::VAO;
	GLuint Quad::VBO;
	GLuint Quad::IBO;
	bool Quad::first = false;

	Quad::Quad(Shader *shader, glm::vec3 points[], glm::vec4 color) {
		mShader = shader; mColor = color;
		mPoints[0] = points[0];
		mPoints[1] = points[1];
		mPoints[2] = points[2];
		mPoints[3] = points[3];
		if (!first) { init(); first = true; }
	}

	void Quad::init() {
		//Generate VAO and bind
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Generate VBO and bind
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, NULL, GL_DYNAMIC_DRAW);

		//Generate IBO and bind
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

		//Attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Quad::render() {
		mShader->enable();
		mShader->setUniform4f("color", mColor);

		GLfloat vertices[] = {
			mPoints[0].x, mPoints[0].y, mPoints[0].z,
			mPoints[1].x, mPoints[1].y, mPoints[1].z,
			mPoints[2].x, mPoints[2].y, mPoints[2].z,
			mPoints[3].x, mPoints[3].y, mPoints[3].z
		};

		GLushort indices[]{
			0, 1, 2,
			0, 2, 3
		};

		//Bind
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		//Unbind
		glBindVertexArray(0);
	}

}