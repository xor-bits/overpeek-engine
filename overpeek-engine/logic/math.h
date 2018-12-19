#pragma once

namespace logic {
	
	inline bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}
	template<class T>
	inline T map(T value, T low1, T high1, T low2, T high2) {
		return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
	}

}