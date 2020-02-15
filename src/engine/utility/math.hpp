#pragma once

#include "engine/internal_libs.hpp"
#include <string>



namespace oe::utils {

	glm::vec2 randomVec2(float min = -1.0f, float max = 1.0f);
	glm::vec3 randomVec3(float min = -1.0f, float max = 1.0f);
	glm::vec4 randomVec4(float min = -1.0f, float max = 1.0f);

	void rotatePoint(glm::vec2 center, float angle, glm::vec2 &p);
	void rotatePoint(float angle, glm::vec2& p);

	template<class T>
	T clamp(const T _val, const T _min, const T _max) {
		return std::max(std::min(_val, _max), _min);
	}

	template<class T>
	T map(const T value, const T low1, const T high1, const T low2, const T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

	std::string boolToString(bool b);

	bool isInRange(int input, int min, int max);

	int sign(float n);

	glm::vec4 rgb2hsv(glm::vec4 in);

	glm::vec4 hsv2rgb(glm::vec4 in);
}
