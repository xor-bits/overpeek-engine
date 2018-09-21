#include "random.h"

#include <cstdlib>
#include <ctime>

namespace tools {

	void Random::seed() {
		srand(static_cast<float>(time(0)));
	}

	float Random::random(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

}