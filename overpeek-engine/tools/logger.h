#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <ostream>
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

		template <class T>
		static void info(T output) {
			m_console->info(output);
		}

		template <class T>
		static void warning(T output) {
			m_console->warn(output);
		}

		template <class T>
		static void critical(T output) {
			m_console->critical(output);
		}

		template <class T>
		static void error(T output) {
			m_console->error(output);
			system("pause");
			glfwTerminate();
			exit(-1);
		}

	};
}