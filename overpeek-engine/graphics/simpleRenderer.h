#pragma once

#include <GL/glew.h>
#include <stddef.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"
#include "fontLoader.h"
#include "renderables/renderable.h"

namespace graphics {

	class SimpleRenderer {
	private:
		std::vector<Renderable*> renderList;

	public:
		void submit(Renderable *renderable);
		void render();

	};
	
}