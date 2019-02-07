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
			spdlog::set_pattern("%^[%l]: %v%$");
			m_initalized = true;
			info("Logger created.");
		}

		template <class T>
		static void info(T output) {
			setup();
			m_console->info(output);
		}

		template <class T1, class T2>
		static void info(T1 output1, T2 output2) {
			setup();
			m_console->info(std::to_string(output1) + ", " + std::to_string(output2));
		}

		template <class T1, class T2, class T3>
		static void info(T1 output1, T2 output2, T3 output3) {
			setup();
			m_console->info(std::to_string(output1) + ", " + std::to_string(output2) + ", " + std::to_string(output3));
		}

		template <class T>
		static void warning(T output) {
			setup();
			m_console->warn(output);
		}

		template <class T>
		static void critical(T output) {
			setup();
			m_console->critical(output);
		}

		template <class T>
		static void error(T output) {
			setup();
			m_console->error(output);
			system("pause");
			glfwTerminate();
			exit(-1);
		}

	};
}