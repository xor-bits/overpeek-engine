#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace tools {
	class Logger {
	private:
		static std::shared_ptr<spdlog::logger> m_console;
		static bool m_initalized;

	public:
		static void setup() {
			if (m_initalized) return;
			m_console = spdlog::stdout_color_mt("console");
			info("Logger created.");
			m_initalized = true;
		}

		static void info(std::string output) {
			m_console->info(output);
		}

		static void warning(std::string output) {
			m_console->warn(output);
		}

		static void critical(std::string output) {
			m_console->critical(output);
		}

		static void error(std::string output) {
			m_console->error(output);
		}

	};
}