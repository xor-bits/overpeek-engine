#pragma once

#include <string>
#include <glm/glm.hpp>

namespace oe {

	class Logger {
	public:
		static void setup();

		template<class T> static void info(T output);
		template<class T> static void debug(T output);
		template<class T> static void warning(T output);
		template<class T> static void critical(T output);
		template<class T> static void error(T output);

		template<typename ... Args> static void info(const char* format, const Args& ... arguments);
		template<typename ... Args> static void debug(const char* format, const Args& ... arguments);
		template<typename ... Args> static void warning(const char* format, const Args& ... arguments);
		template<typename ... Args> static void critical(const char* format, const Args& ... arguments);
		template<typename ... Args> static void error(const char* format, const Args& ... arguments);

		//template<typename ... T> static void info(T ... output);
		//template<typename ... T> static void debug(T ... output);
		//template<typename ... T> static void warning(T ... output);
		//template<typename ... T> static void critical(T ... output);
		//template<typename ... T> static void error(T ... output);

	};

}