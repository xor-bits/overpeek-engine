#pragma once

#include "engine/internal_libs.hpp"
#include <string>



namespace oe::utils {

	extern void rotatePoint(glm::vec2 center, float angle, glm::vec2& p);
	extern void rotatePoint(float angle, glm::vec2& p);

	extern std::string boolToString(bool b);
	extern bool isInRange(int input, int min, int max);
	extern int sign(float n);

	template<class T>
	T clamp(const T _val, const T _min, const T _max) {
		return std::max(std::min(_val, _max), _min);
	}

	template<class T>
	T map(const T value, const T low1, const T high1, const T low2, const T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

    // rgb/hsv algorthms by
    // https://stackoverflow.com/a/6930407/12147216

	extern glm::vec3 rgb_to_hsv(glm::vec3 in);
	extern glm::vec3 hsv_to_rgb(glm::vec3 in);

}
