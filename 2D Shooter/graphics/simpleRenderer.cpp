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

<<<<<<< HEAD
		glGenVertexArrays(1, &mVAOPoint);
		glGenBuffers(1, &mVBOPoint);
		glBindVertexArray(mVAOPoint);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOPoint);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 2, NULL, GL_DYNAMIC_DRAW);
=======
		glGenVertexArrays(1, &mVAOLn);
		glGenBuffers(1, &mVBOLn);
		glBindVertexArray(mVAOLn);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOLn);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 2, NULL, GL_DYNAMIC_DRAW);
>>>>>>> 64784bb13d14e080ca926845f21fc4c6be53f0f4
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
<<<<<<< HEAD
	}

	void SimpleRenderer::renderPoints(std::vector<glm::vec2> points, glm::vec4 color) {

=======
>>>>>>> 64784bb13d14e080ca926845f21fc4c6be53f0f4
	}

	void SimpleRenderer::renderTriangle(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color) {

		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		mShader->SetUniformMat4("ml_matrix", modelMatrix);
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

	void SimpleRenderer::renderTriangle(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color) {

		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		mShader->SetUniformMat4("ml_matrix", modelMatrix);
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

	void SimpleRenderer::renderQuad(glm::mat4 modelMatrix, float x, float y, float w, float h, glm::vec4 color) {
		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		mShader->SetUniformMat4("ml_matrix", modelMatrix);
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

	void SimpleRenderer::renderQuad(glm::mat4 modelMatrix, float x, float y, std::vector<glm::vec2> points, glm::vec4 color) {
		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		mShader->SetUniformMat4("ml_matrix", modelMatrix);
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

	void SimpleRenderer::renderCircle(glm::mat4 modelMatrix, float x, float y, float r, unsigned int resolution, glm::vec4 color) {

		std::vector<glm::vec2> points;
		points.push_back(glm::vec2(x, y));
		points.push_back(glm::vec2(x, y));
		points.push_back(glm::vec2(x, y));

		float pi2ByResolution = glm::two_pi<float>() / resolution;

		for (int i = 0; i < resolution; i++) {
			points[1] = glm::vec2(r * sin(pi2ByResolution * i), r * cos(pi2ByResolution * i));
			points[2] = glm::vec2(r * sin(pi2ByResolution * (i + 1)), r * cos(pi2ByResolution * (i+1)));
			renderTriangle(modelMatrix, 0.0f, 0.0f, points, color);
		}
	}

	void SimpleRenderer::renderLine(glm::mat4 modelMatrix, float w, glm::vec2 A, glm::vec2 B, glm::vec4 color) {
		// Activate corresponding render state
		mShader->enable();
		mShader->setUniform4f("color", color);
		mShader->SetUniformMat4("ml_matrix", modelMatrix);
		glBindVertexArray(mVAOLn);
		glLineWidth(w);

		GLfloat vertices[2][2] = {
			{ A.x, A.y },
			{ B.x, B.y }
		};

		//Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBOLn);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Render quad
		glDrawArrays(GL_LINES, 0, 2);

		//Unbind
		glBindVertexArray(0);


	}

}