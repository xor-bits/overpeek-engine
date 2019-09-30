#include "debug.h"
#include "logger.h"
#include "clock.h"
#include "../graphics/window.h"

#include <string>

namespace oe {

	long long Debug::m_passed_time;

	void Debug::startTimer() {
		m_passed_time = oe::Clock::getMicroseconds();
	}

	int Debug::getTimer() {
		return oe::Clock::getMicroseconds() - m_passed_time;
	}

	void Debug::printTimer() {
		oe::Logger::info("Passed time in microseconds: " + std::to_string(getTimer()));
	}

	void Debug::printTimer(std::string text) {
		oe::Logger::info(text + std::to_string(getTimer()));
	}

	void Debug::testGLErrors(std::string errortext) {
		oe::Logger::debug(errortext + " : " + std::to_string(oe::Window::checkGLErrors()));
	}
}