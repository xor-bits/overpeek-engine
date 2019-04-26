#pragma once

#include <string>
#include <glm/glm.hpp>

namespace oe {
	//not in logger
	static enum outType {
		info, warning, critical, error
	};

	class Logger {
	private:
		static void *m_console;
		static bool m_initalized;

	public:

		static void setup();

		static void out(double output, int type = info);
		static void out(const char *output, int type = info);
		static void out(std::string output, int type = info);
		static void out(double output, double output2, int type = info);
		static void out(const char *output, const char *output2, int type = info);
		static void out(const char *output, double output2, int type = info);
		static void out(const char *output, double output2, double output3, int type = info);
		static void out(const char *output, const char *output2, const char *output3, int type = info);
		static void out(const char *output, double output2, const char *output3, double output4, int type = info);
		static void out(const char *output, double output2, const char *output3, double output4, const char *output5, double output6, int type = info);

		static void out(glm::vec2 output, int type = info);
		static void out(const char* output, glm::vec2 output2, int type = info);
		static void out(glm::vec3 output, int type = info);
		static void out(const char* output, glm::vec3 output2, int type = info);
		static void out(glm::vec4 output, int type = info);
		static void out(const char* output, glm::vec4 output2, int type = info);

	};
}