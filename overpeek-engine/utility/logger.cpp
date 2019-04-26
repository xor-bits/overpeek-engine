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
		out("Logger created.", info);
	}

	void Logger::out(std::string output, int type) {
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

	void Logger::out(const char *output, int type) {
		std::string log = output;
		out(log, type);
	}

	void Logger::out(double output, int type) {
		std::string log = std::to_string(output);
		out(log, type);
	}

	void Logger::out(double output, double output2, int type) {
		std::string log = std::to_string(output) + ", " + std::to_string(output2);
		out(log, type);
	}

	void Logger::out(const char *output, double output2, int type) {
		std::string log = output + std::to_string(output2);
		out(log, type);
	}

	void Logger::out(const char *output, double output2, double output3, int type) {
		std::string log = output + std::to_string(output2) + ", " + std::to_string(output3);
		out(log, type);
	}

	void Logger::out(const char *output, const char *output2, int type) {
		std::string log = std::string(output) + output2;
		out(log, type);
	}

	void Logger::out(const char *output, const char *output2, const char *output3, int type) {
		std::string log = std::string(output) + output2 + output3;
		out(log, type);
	}

	void Logger::out(const char *output, double output2, const char *output3, double output4, int type) {
		std::string log = std::string(output) + std::to_string(output2) + output3 + std::to_string(output4);
		out(log, type);
	}

	void Logger::out(const char *output, double output2, const char *output3, double output4, const char *output5, double output6, int type) {
		std::string log = std::string(output) + std::to_string(output2) + output3 + std::to_string(output4) + output5 + std::to_string(output6);
		out(log, type);
	}

	void Logger::out(glm::vec2 output, int type) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y);
		out(log, type);
	}

	void Logger::out(const char* output, glm::vec2 output2, int type) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y);
		out(log, type);
	}

	void Logger::out(glm::vec3 output, int type) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y) + ", " + std::to_string(output.z);
		out(log, type);
	}

	void Logger::out(const char* output, glm::vec3 output2, int type) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y) + ", " + std::to_string(output2.z);
		out(log, type);
	}

	void Logger::out(glm::vec4 output, int type) {
		std::string log = std::to_string(output.x) + ", " + std::to_string(output.y) + ", " + std::to_string(output.z) + ", " + std::to_string(output.w);
		out(log, type);
	}
	
	void Logger::out(const char* output, glm::vec4 output2, int type) {
		std::string log = output + std::to_string(output2.x) + ", " + std::to_string(output2.y) + ", " + std::to_string(output2.z) + ", " + std::to_string(output2.z);
		out(log, type);
	}

}