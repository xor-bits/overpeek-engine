#include "quad.h"

namespace graphics {

	Quad::Quad(Shader *shader, glm::vec3 points[], glm::vec4 color) {
		mShader = shader; mColor = color;
		mPoints[0] = points[0];
		mPoints[1] = points[1];
		mPoints[2] = points[2];
		mPoints[3] = points[3];

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

		mVAO = VertexArray();
		mVBO = new VertexBuffer(vertices, 12, 3);
		mIBO = new IndexBuffer(indices, 6);
		mVAO.addBuffer(mVBO, 0);
	}

	void Quad::render() {
		mShader->enable();
		mShader->setUniform4f("color", mColor);

		mVAO.bind();
		mIBO->bind();
		glDrawElements(GL_TRIANGLES, mIBO->getCount(), GL_UNSIGNED_SHORT, 0);
		mIBO->unbind();
		mVAO.unbind();
	}

}