#include "math.h"

#include "random.h"



namespace oe::utils {

	glm::vec2 randomVec2(float min, float max) {
		return glm::vec2(oe::Random::random(min, max), oe::Random::random(min, max));
	}

	glm::vec3 randomVec3(float min, float max) {
		return glm::vec3(oe::Random::random(min, max), oe::Random::random(min, max), oe::Random::random(min, max));
	}
	
	glm::vec4 randomVec4(float min, float max) {
		return glm::vec4(oe::Random::random(min, max), oe::Random::random(min, max), oe::Random::random(min, max), oe::Random::random(min, max));
	}

	void rotatePoint(glm::vec2 center, float angle, glm::vec2& p) {
		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		p.x -= center.x;
		p.y -= center.y;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + center.x;
		p.y = ynew + center.y;
	}

	void rotatePoint(float angle, glm::vec2& p) {
		float s = sin(angle);
		float c = cos(angle);

		// rotate point
		p = { (p.x * c - p.y * s), (p.x * s + p.y * c) };
	}

	void rotatePoint(glm::vec3 center, float xyangle, float xzangle, glm::vec3& p) {
		float s = sin(xyangle);
		float c = cos(xyangle);

		float s2 = sin(xzangle);
		float c2 = cos(xzangle);

		// translate point back to origin:
		p.x -= center.x;
		p.y -= center.y;
		p.z -= center.z;

		// rotate point
		float xnew = p.x * c * c2 - p.y * s - p.z * s2;
		float ynew = p.x * s + p.y * c;
		float znew =  p.x * s2 + p.z * c2;

		// translate point back:
		p.x = xnew + center.x;
		p.y = ynew + center.y;
		p.z = znew + center.z;
	}

	std::string boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}

	int sign (float n) {
		if (n > 0) return 1;
		return -1;
	}
}