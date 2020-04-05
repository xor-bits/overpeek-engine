#pragma once

#include <cstddef>

namespace oe::utils {

	class Clock {
	private:
		size_t m_start_time;

	public:
		size_t getMicroseconds();
		size_t getTimeAfterStart();
		float getSessionMillisecond();

	private:
		static Clock* singleton;
		Clock();

	public:
		Clock(const Clock&) = delete;
		static Clock& getSingleton() {
			if (!singleton) singleton = new Clock();
			return *singleton;
		}
	};

}
