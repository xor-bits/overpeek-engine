#pragma once

namespace oe {

	class Clock {
	private:
		static long long m_start_time;
	public:
		static long long getMicroseconds();
		static int getTimeAfterStart();
	};

}