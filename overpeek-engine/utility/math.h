#pragma once

#include <glm/glm.hpp>
#include <string>

namespace oe {

	#define min(a, b) (a > b ? b : a)
	#define max(a, b) (a > b ? a : b)

	//XY angle
	void rotatePoint(glm::vec2 center, float xyangle, glm::vec2 &p);
	
	//XY angle and XZ angle
	void rotatePoint(glm::vec3 center, float xyangle, float xzangle, glm::vec3 &p);

	template<class T>
	T clamp(T _val, T _min, T _max) {
		return max(min(_val, _max), _min);
	}

	template<class T>
	T map(T value, T low1, T high1, T low2, T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

	std::string boolToString(bool b);

	bool isInRange(int input, int min, int max);

}