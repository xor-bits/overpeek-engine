#pragma once


#include "renderable.h"

namespace graphics {

	class Quad : public Renderable{
	private:
		VertexArray mVAO;
		VertexBuffer *mVBO;
		IndexBuffer *mIBO;

		glm::vec3 mPoints[];

	public:
		Quad(Shader *shader, glm::vec3 points[], glm::vec4 color);

		void render() override;

		glm::vec3 getPoints(GLuint index) { return mPoints[index]; }
	};

}

