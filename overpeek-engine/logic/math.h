#pragma once

namespace logic {
	
	inline bool isInRange(int input, int min, int max) {
		return (input >= min) && (input <= max);
	}

}