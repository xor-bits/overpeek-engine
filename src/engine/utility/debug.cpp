#include "debug.hpp"
#include "clock.hpp"
#include "engine/internal_libs.hpp"

#include <string>



namespace oe::utils {

	long long Debug::m_passed_time;

	void Debug::startTimer() {
		m_passed_time = oe::utils::Clock::getMicroseconds();
	}

	int Debug::getTimer() {
		return oe::utils::Clock::getMicroseconds() - m_passed_time;
	}

	void Debug::printTimer() {
		spdlog::info("Passed time in microseconds: " + std::to_string(getTimer()));
	}

	void Debug::printTimer(std::string text) {
		spdlog::info(text + std::to_string(getTimer()));
	}
}