#pragma once

namespace oe::utils {

	class Clock {
	private:
		static long long m_start_time;
	public:
		static long long getMicroseconds();
		static int getTimeAfterStart();
	};

}