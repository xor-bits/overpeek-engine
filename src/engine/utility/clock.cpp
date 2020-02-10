#include "clock.h"

#include <chrono>

using namespace std::chrono;



namespace oe::utils {

	size_t Clock::m_start_time = getMicroseconds();

	size_t Clock::getMicroseconds() {
		auto now = system_clock::now();
		auto now_ns = time_point_cast<microseconds>(now);
		auto epoch = now_ns.time_since_epoch();
		auto value = duration_cast<microseconds>(epoch);
		return value.count();
	}

	size_t Clock::getTimeAfterStart() {
		return getMicroseconds() - m_start_time;
	}

	float Clock::getSessionMillisecond() {
		return getTimeAfterStart() * 0.001f;
	}

}