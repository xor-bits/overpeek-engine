#pragma once

#include <string>

namespace tools {

	class Debug {
	private:
		static long long m_passed_time;

	public:
		static void startTimer();
		static long long getTimer();
		static void printTimer();
		static void printTimer(std::string text);
	};

}