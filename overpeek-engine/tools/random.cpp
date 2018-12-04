#include "random.h"

#include <cstdlib>
#include <ctime>

namespace tools {

	void Random::seed() {
		srand(static_cast<float>(time(0)));
	}

	void Random::seed(int seed) {
		srand(static_cast<float>(seed));
	}

	float Random::random(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	unsigned long Random::hash(const char* str) {
		unsigned long h = PRIME_H;
		while (*str) {
			h = (h * PRIME_A) ^ (str[0] * PRIME_B);
			str++;
		}
		return h % PRIME_C;
	}

}