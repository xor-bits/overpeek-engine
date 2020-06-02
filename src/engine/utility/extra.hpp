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

	extern glm::vec3 rgbToHSV(glm::vec3 in);
	extern glm::vec3 hsvToRGB(glm::vec3 in);

	// hex conversions
	extern glm::ivec3 hexToRGB(unsigned long hex);
	extern unsigned long RGBtoHex(glm::ivec3 rgb);
	extern unsigned long stringToHex(const std::string& str);
	extern std::string hexToString(unsigned long hex);

	// string checks
	extern bool checkChar(const std::string& text, char character, int position);
	extern bool isNumber(const char* input);
	extern bool isNumber(const std::string& input);

}
