#include "math.h"

namespace oe {

	void rotatePoint(glm::vec2 center, float xyangle, glm::vec2& p) {
		float s = sin(xyangle);
		float c = cos(xyangle);

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

}