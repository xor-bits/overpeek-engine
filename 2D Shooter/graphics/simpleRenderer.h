#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <glm/glm.hpp>

#include "shader.h"


namespace graphics {

	class SimpleRenderer {
	private:
		GLuint mVAO, mVBO;

	public:
		SimpleRenderer();

		void renderQuad(Shader &shader, float x, float y, float w, float h, glm::vec4 color);

	};

}