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

	void Logger::out(int type, std::string output) {
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

	void Logger::out(int type, const char *output) {
		std::string log = output;
		out(type, log);
	}

	void Logger::out(int type, double output) {
		std::string log = std::to_string(output);
		out(type, log);
	}

	void Logger::out(int type, const char *output, double output2) {
		std::string log = output + std::to_string(output2);
		out(type, log);
	}

	void Logger::out(int type, const char *output, double output2, double output3) {
		std::string log = output + std::to_string(output2) + ", " + std::to_string(output3);
		out(type, log);
	}

	void Logger::out(int type, const char *output, const char *output2) {
		std::string log = std::string(output) + output2;
		out(type, log);
	}

	void Logger::out(int type, const char *output, const char *output2, const char *output3) {
		std::string log = std::string(output) + output2 + output3;
		out(type, log);
	}

	void Logger::out(int type, const char *output, double output2, const char *output3, double output4) {
		std::string log = std::string(output) + std::to_string(output2) + output3 + std::to_string(output4);
		out(type, log);
	}

	void Logger::out(int type, const char *output, double output2, const char *output3, double output4, const char *output5, double output6) {
		std::string log = std::string(output) + std::to_string(output2) + output3 + std::to_string(output4) + output5 + std::to_string(output6);
		out(type, log);
	}

	void Logger::out(int type, glm::vec2 output) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y);
		out(type, log);
	}

	void Logger::out(int type, const char* output, glm::vec2 output2) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y);
		out(type, log);
	}

	void Logger::out(int type, glm::vec3 output) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y) + ", " + std::to_string(output.z);
		out(type, log);
	}

	void Logger::out(int type, const char* output, glm::vec3 output2) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y) + ", " + std::to_string(output2.z);
		out(type, log);
	}

	void Logger::out(int type, glm::vec4 output) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y) + ", " + std::to_string(output.z) + ", " + std::to_string(output.w);
		out(type, log);
	}
	
	void Logger::out(int type, const char* output, glm::vec4 output2) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y) + ", " + std::to_string(output2.z) + ", " + std::to_string(output2.z);
		out(type, log);
	}

}