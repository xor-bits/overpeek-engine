#include "logger.h"

namespace oe {

	std::shared_ptr<spdlog::logger> Logger::m_console;
	bool Logger::m_initalized;

}