#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "../shader.h"

namespace graphics {

	class Renderable {
	protected:
		Shader *mShader;
		glm::vec4 mColor;

	public:
		virtual void render() {};

	};

}