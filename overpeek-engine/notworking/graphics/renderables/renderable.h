#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "../shader.h"
#include "../buffers/indexBuffer.h"
#include "../buffers/vertexBuffer.h"
#include "../buffers/vertexArray.h"

namespace graphics {

	class Renderable {
	protected:
		Shader *mShader;
		glm::vec3 mPos;
		glm::vec4 mColor;

	public:
		virtual void render() {};

	};

}