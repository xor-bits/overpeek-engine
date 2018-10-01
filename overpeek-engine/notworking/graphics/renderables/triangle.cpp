#include "triangle.h"

namespace graphics {

	Triangle::Triangle(Shader *shader, glm::vec3 points[], glm::vec4 color) {
		mShader = shader; mColor = color;

		GLfloat vertices[] = {
			points[0].x, points[0].y, points[0].z,
			points[1].x, points[1].y, points[1].z,
			points[2].x, points[2].y, points[2].z
		};

		GLushort indices[]{
			0, 1, 2
		};

		mVAO = VertexArray();
		mVBO = new VertexBuffer(vertices, 9, 3);
		mIBO = new IndexBuffer(indices, 3);
		mVAO.addBuffer(mVBO, 0);
	}

	void Triangle::render() {
		mShader->enable();
		mShader->setUniform4f("color", mColor);

		mVAO.bind();
		mIBO->bind();
		glDrawElements(GL_TRIANGLES, mIBO->getCount(), GL_UNSIGNED_SHORT, 0);
		mIBO->unbind();
		mVAO.unbind();
	}

}