#pragma once

#include <cstddef>

namespace oe::utils {

	class Clock {
	private:
		static size_t m_start_time;
	public:
		static size_t getMicroseconds();
		static size_t getTimeAfterStart();

		static float getSessionMillisecond();
	};

}
