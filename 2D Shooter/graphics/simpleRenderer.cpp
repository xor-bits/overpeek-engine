#include "simpleRenderer.h"


namespace graphics {

	SimpleRenderer::SimpleRenderer(Shader &shader) {
		mShader = &shader;

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

	void SimpleRenderer::renderTriangle(float x, float y, float w, float h, glm::vec4 color) {

		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		glBindVertexArray(mVAOTri);

		GLfloat vertices[3][2] = {
				{ x + w / 2,  y + h },
				{ x        ,  y     },
				{ x + w    ,  y     }
		};

		//Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBOTri);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind
		glBindVertexArray(0);
	}

	void SimpleRenderer::renderTriangle(float x, float y, std::vector<glm::vec2> points, glm::vec4 color) {

		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		glBindVertexArray(mVAOTri);

		GLfloat vertices[3][2] = {
				{ x + points[0].x, y + points[0].y },
				{ x + points[1].x, y + points[1].y },
				{ x + points[2].x, y + points[2].y }
		};

		//Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBOTri);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Unbind
		glBindVertexArray(0);
	}

	void SimpleRenderer::renderQuad(float x, float y, float w, float h, glm::vec4 color) {
		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
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

	void SimpleRenderer::renderQuad(float x, float y, std::vector<glm::vec2> points, glm::vec4 color) {
		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		glBindVertexArray(mVAOQuad);

		GLfloat vertices[6][2] = {
				{ x + points[0].x, y + points[0].y },
				{ x + points[1].x, y + points[1].y },
				{ x + points[2].x, y + points[2].y },
				{ x + points[3].x, y + points[3].y },
				{ x + points[4].x, y + points[4].y },
				{ x + points[5].x, y + points[5].y }
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