#pragma once

#include "renderable.h"

namespace graphics {

	class Triangle : public Renderable {
	private:
		VertexArray mVAO;
		VertexBuffer *mVBO;
		IndexBuffer *mIBO;
	public:
		Triangle(Shader *shader, glm::vec3 points[], glm::vec4 color);

		void render() override;
	};

}