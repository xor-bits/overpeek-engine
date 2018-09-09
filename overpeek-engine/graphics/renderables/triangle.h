#pragma once

#include "renderable.h"

namespace graphics {

	class Triangle : public Renderable {
	private:
		glm::vec3 mPoints[3];
		static GLuint VAO, VBO, IBO;
		static bool first;

		static void init();
	public:
		Triangle(Shader *shader, glm::vec3 points[], glm::vec4 color);

		void render() override;
	};

}