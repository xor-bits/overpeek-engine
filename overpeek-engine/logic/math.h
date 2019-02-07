#pragma once

#include <string>
#include <glm/glm.hpp>
#include <algorithm>

namespace logic {
	
	template<class T>
	inline T clamp(T _val, T _min, T _max) {
		return std::max(std::min(_val, _max), _min);
	}
	inline bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}
	template<class T>
	inline T map(T value, T low1, T high1, T low2, T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}
	inline std::string const boolToString(bool b)
	{
		return b ? "true" : "false";
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