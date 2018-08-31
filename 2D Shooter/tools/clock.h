#pragma once


#include <chrono>

namespace tools {

	using namespace std::chrono;

	long long getMicroseconds() {
		auto now = system_clock::now();
		auto now_ns = time_point_cast<microseconds>(now);
		auto epoch = now_ns.time_since_epoch();
		auto value = duration_cast<microseconds>(epoch);
		return value.count();
	}

}