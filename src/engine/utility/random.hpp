#pragma once

#include <string>
#include "engine/internal_libs.hpp"



namespace oe {

	class Random {
	public:
		//Seed = 0 is time
		static void seed(int seed = 0);
		static float random(float min, float max);
		static glm::vec2 randomVec2(float min, float max);
		static glm::vec3 randomVec3(float min, float max);
		static glm::vec4 randomVec4(float min, float max);
	};

}