#include "logger.h"

#include "../graphics/window.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace oe {

	void *Logger::m_console;
	bool Logger::m_initalized;

	void Logger::setup() {
		if (m_initalized) return;
		m_console = spdlog::stdout_color_mt("console").get();
		spdlog::set_pattern("%^[%l]: %v%$");
		m_initalized = true;
		out(info, "Logger created.");
	}

	void Logger::out(int type, const char *output) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(output);
			break;
		case warning:
			console->warn(output);
			break;
		case critical:
			console->critical(output);
			break;
		case error:
			console->error(output);
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, double output) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(output);
			break;
		case warning:
			console->warn(output);
			break;
		case critical:
			console->critical(output);
			break;
		case error:
			console->error(output);
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, const char *output, double output2) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(output + std::to_string(output2));
			break;
		case warning:
			console->warn(output + std::to_string(output2));
			break;
		case critical:
			console->critical(output + std::to_string(output2));
			break;
		case error:
			console->error(output + std::to_string(output2));
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, const char *output, double output2, double output3) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(output + std::to_string(output2) + ", " + std::to_string(output3));
			break;
		case warning:
			console->warn(output + std::to_string(output2) + ", " + std::to_string(output3));
			break;
		case critical:
			console->critical(output + std::to_string(output2) + ", " + std::to_string(output3));
			break;
		case error:
			console->error(output + std::to_string(output2) + ", " + std::to_string(output3));
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, const char *output, const char *output2) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(std::string(output) + output2);
			break;
		case warning:
			console->warn(std::string(output) + output2);
			break;
		case critical:
			console->critical(std::string(output) + output2);
			break;
		case error:
			console->error(std::string(output) + output2);
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, const char *output, const char *output2, const char *output3) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(std::string(output) + output2 + output3);
			break;
		case warning:
			console->warn(std::string(output) + output2 + output3);
			break;
		case critical:
			console->critical(std::string(output) + output2 + output3);
			break;
		case error:
			console->error(std::string(output) + output2 + output3);
			Window::terminate();
			break;

		default:
			break;
		}
	}

	void Logger::out(int type, const char *output, double output2, const char *output3, double output4) {
		setup();
		spdlog::logger *console = (spdlog::logger*)m_console;

		switch (type)
		{
		case info:
			console->info(std::string(output) + std::to_string(output2) + output3 + std::to_string(output4));
			break;
		case warning:
			console->warn(std::string(output) + std::to_string(output2) + output3 + std::to_string(output4));
			break;
		case critical:
			console->critical(std::string(output) + std::to_string(output2) + output3 + std::to_string(output4));
			break;
		case error:
			console->error(std::string(output) + std::to_string(output2) + output3 + std::to_string(output4));
			Window::terminate();
			break;

		default:
			break;
		}
	}

}