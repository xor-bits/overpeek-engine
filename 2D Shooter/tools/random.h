#pragma once


#include <cstdlib>
#include <ctime>

namespace tools {

	void seed() {
		srand(static_cast<float>(time(0)));
	}

	float random(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

}