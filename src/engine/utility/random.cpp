#include "random.h"

#include <cstdlib>
#include <ctime>

namespace oe {

	void Random::seed(int seed) {
		if (seed == 0) seed = time(0);
		srand(static_cast<float>(seed));
	}

	float Random::random(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	glm::vec2 Random::randomVec2(float min, float max) {
		return glm::vec2(random(min, max), random(min, max));
	}

	glm::vec3 Random::randomVec3(float min, float max) {
		return glm::vec3(random(min, max), random(min, max), random(min, max));
	}

	glm::vec4 Random::randomVec4(float min, float max) {
		return glm::vec4(random(min, max), random(min, max), random(min, max), random(min, max));
	}

}