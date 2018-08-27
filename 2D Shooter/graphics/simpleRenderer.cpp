#include "simpleRenderer.h"


namespace graphics {

	SimpleRenderer::SimpleRenderer() {
		glGenVertexArrays(1, &mVAOQuad);
		glGenBuffers(1, &mVBOQuad);
		glBindVertexArray(mVAOQuad);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenVertexArrays(1, &mVAOTri);
		glGenBuffers(1, &mVBOTri);
		glBindVertexArray(mVAOTri);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOTri);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 2, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SimpleRenderer::renderQuad(Shader &shader, float x, float y, float w, float h, glm::vec4 color) {

		// Activate corresponding render state
		shader.enable();
<<<<<<< HEAD
		shader.setUniform4f("testColor", color);
		glBindVertexArray(mVAOQuad);

		GLfloat vertices[6][2] = {
				{ x - w / 2,  y + h / 2 },
				{ x - w / 2,  y - h / 2 },
				{ x + w / 2,  y - h / 2 },
				{ x - w / 2,  y + h / 2 },
				{ x + w / 2,  y - h / 2 },
				{ x + w / 2,  y + h / 2 }
		};

		//Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBOQuad);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Unbind
		glBindVertexArray(0);
	}

	void SimpleRenderer::renderTriangle(Shader &shader, float x, float y, float w, float h, glm::vec4 color) {
		// Activate corresponding render state
		shader.enable();
		shader.setUniform4f("testColor", color);
		glBindVertexArray(mVAOQuad);
=======
		shader.setUniform4f("color", color);
		glBindVertexArray(mVAO);
>>>>>>> e104e42458129b39e86597a09d16e0480e24bd4b

		GLfloat vertices[6][2] = {
				{ x - w / 2,  y + h / 2 },
				{ x - w / 2,  y - h / 2 },
				{ x + w / 2,  y - h / 2 },
				{ x - w / 2,  y + h / 2 },
				{ x + w / 2,  y - h / 2 },
				{ x + w / 2,  y + h / 2 }
		};

		//Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBOQuad);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Unbind
		glBindVertexArray(0);
	}

}