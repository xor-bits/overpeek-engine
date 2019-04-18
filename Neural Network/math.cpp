#include "math.h"

#include <cmath>
#define PI 3.14159f

//(5 / sqrt(2 * PI))
#define SQRTPART 1.99471f
#define E 2.71828

namespace math {
	
	float gaussRandom(float min, float max) {
		float x = random(-2.4, 2.4);
		SQRTPART * pow(E, -(pow(x, 2) / 2));
	}

	float random(float min, float max) {
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	float sigmoid(float val) {
		return ((1) / (1 + pow(e, -val)));
	}

	float relu(float val) {
		return val > 0.0f ? val : 0.0f;
	}

	float sigmoidDerivate(float val) {
		float sig = sigmoid(val);
		return (sig) * (1 - sig);
	}

	float reluDerivate(float val) {
		return val > 0.0f ? 1.0f : 0.0f;
	}
}