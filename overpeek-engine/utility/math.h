#pragma once

#include "../internal_libs.h"
#include <string>

namespace oe {

	#define min(a, b) (a > b ? b : a)
	#define max(a, b) (a > b ? a : b)

	//XY angle
	void rotatePoint(glm::vec2 center, float xyangle, glm::vec2 &p);

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

	int sign(float n);

	glm::vec4 rgb2hsv(glm::vec4 in);

	glm::vec4 hsv2rgb(glm::vec4 in);
}