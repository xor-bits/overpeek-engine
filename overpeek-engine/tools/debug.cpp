#include "debug.h"
#include "logger.h"
#include "clock.h"

namespace tools {

	long long Debug::m_passed_time;

	void Debug::startTimer() {
		m_passed_time = tools::Clock::getMicroseconds();
	}

	long long Debug::getTimer() {
		return tools::Clock::getMicroseconds() - m_passed_time;
	}

	void Debug::printTimer() {
		tools::Logger::setup();
		tools::Logger::info("Passed time in microseconds: " + std::to_string(getTimer()));
	}

	void Debug::printTimer(std::string text) {
		tools::Logger::setup();
		tools::Logger::info(text + std::to_string(getTimer()));
	}
}