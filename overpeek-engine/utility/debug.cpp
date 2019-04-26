#include "debug.h"
#include "logger.h"
#include "clock.h"

namespace oe {

	long long Debug::m_passed_time;

	void Debug::startTimer() {
		m_passed_time = oe::Clock::getMicroseconds();
	}

	long long Debug::getTimer() {
		return oe::Clock::getMicroseconds() - m_passed_time;
	}

	void Debug::printTimer() {
		oe::Logger::out("Passed time in microseconds: ", getTimer(), oe::info);
	}

	void Debug::printTimer(std::string text) {
		oe::Logger::out(text.c_str(), getTimer(), oe::info);
	}
}