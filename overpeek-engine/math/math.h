#pragma once

#include <string>
#include <glm/glm.hpp>
#include <algorithm>

#define min(a, b) (a > b ? b : a)
#define max(a, b) (a > b ? a : b)

namespace oe {

	//template<class T>
	//inline T min(T _1, T _2) {
	//	return _1 > _2 ? _2 : _1;
	//}
	//
	//template<class T>
	//inline T max(T _1, T _2) {
	//	return _1 > _2 ? _1 : _2;
	//}

	template<class T>
	inline T clamp(T _val, T _min, T _max) {
		return max(min(_val, _max), _min);
	}

	template<class T>
	inline T map(T value, T low1, T high1, T low2, T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

	inline std::string const boolToString(bool b)
	{
		return b ? "true" : "false";
	}

	inline bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}
	//glm::vec2 rotatePoint(float cx, float cy, float angle, glm::vec2 p)
	//{
	//	float s = sin(angle);
	//	float c = cos(angle);
	//
	//	// translate point back to origin:
	//	p.x -= cx;
	//	p.y -= cy;
	//
	//	// rotate point
	//	float xnew = p.x * c - p.y * s;
	//	float ynew = p.x * s + p.y * c;
	//
	//	// translate point back:
	//	p.x = xnew + cx;
	//	p.y = ynew + cy;
	//	return p;
	//}

}