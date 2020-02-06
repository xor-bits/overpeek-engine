#pragma once

#include <string>



namespace oe::utils {

	class Debug {
	private:
		static long long m_passed_time;

	public:
		static void startTimer();
		static int getTimer();
		static void printTimer();
		static void printTimer(std::string text);
		static void testGLErrors(std::string errortext);
	};

}