#pragma once


#include "renderable.h"

namespace graphics {

	class Quad : public Renderable{
	private:
		glm::vec3 mPoints[4];
		static GLuint VAO, VBO, IBO;
		static bool first;

		static void init();

	public:
		Quad(Shader *shader, glm::vec3 points[], glm::vec4 color);

		void render() override;
	};

}

