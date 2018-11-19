#include "logger.h"

namespace tools {

	std::shared_ptr<spdlog::logger> Logger::m_console;
	bool Logger::m_initalized;

}