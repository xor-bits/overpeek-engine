#include "batchRenderer.h"

namespace graphics {

	BatchRenderer::BatchRenderer() {
		init();
	}

	BatchRenderer::~BatchRenderer() {
		delete mIBO;
		glDeleteBuffers(1, &mVBO);
	}

	void BatchRenderer::init() {
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, BATCH_MAX_SPRITE_SIZE, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLushort indices[BATCH_MAX_INDICES];

		int offset = 0;
		for (int i = 0; i < BATCH_MAX_INDICES; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;

			offset += 4;
		}

		mIBO = new IndexBuffer(indices, BATCH_MAX_INDICES);

		glBindVertexArray(0);
	}

	void BatchRenderer::beginSubmits() {
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		mBuffer = (glm::vec3*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void BatchRenderer::submit(Quad *sprite) {
		glm::vec3 points = sprite->getPoints(0);
		mBuffer = &glm::vec3(points.x, points.y, points.z);
		points = sprite->getPoints(1);
		mBuffer = &glm::vec3(points.x, points.y, points.z);
		points = sprite->getPoints(2);
		mBuffer = &glm::vec3(points.x, points.y, points.z);
		points = sprite->getPoints(3);
		mBuffer = &glm::vec3(points.x, points.y, points.z);
		mBuffer++;

		mIndexCount += 6;
	}

	void BatchRenderer::endSubmits() {
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BatchRenderer::render() {
		glBindVertexArray(mVAO);
		mIBO->bind();
		glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, 0);
		mIBO->unbind();
		glBindVertexArray(0);
		mIndexCount = 0;
	}

}