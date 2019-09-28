#include "logger.h"

#include "../graphics/window.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define D template void Logger

namespace oe {

	void Logger::setup() {
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);
	}

	template<class T> void Logger::info(T output) {
		spdlog::info(output);
	}

	template<class T> void Logger::debug(T output) {
		spdlog::debug(output);
	}

	template<class T> void Logger::warning(T output){
		spdlog::warn(output);
	}	

	template<class T> void Logger::critical(T output){
		spdlog::critical(output);
	}

	template<class T> void Logger::error(T output){
		spdlog::error(output);
	}

	template<typename ... Args> void Logger::info(const char* format, const Args& ... arguments) {
		spdlog::info(format, arguments...);
	}

	template<typename ... Args> void Logger::debug(const char* format, const Args& ... arguments) {
		spdlog::debug(format, arguments...);
	}

	template<typename ... Args> void Logger::warning(const char* format, const Args& ... arguments) {
		spdlog::warn(format, arguments...);
	}

	template<typename ... Args> void Logger::critical(const char* format, const Args& ... arguments) {
		spdlog::critical(format, arguments...);
	}

	template<typename ... Args> void Logger::error(const char* format, const Args& ... arguments) {
		spdlog::error(format, arguments...);
	}

	// declarations

	// const char*
	D::info			(const char*);
	D::debug		(const char*);
	D::warning		(const char*);
	D::critical		(const char*);
	D::error		(const char*);

	D::info<int>	(const char*, int const &);
	D::debug		(const char*);
	D::warning		(const char*);
	D::critical		(const char*);
	D::error		(const char*);
	
	// string
	D::info			(std::string);
	D::debug		(std::string);
	D::warning		(std::string);
	D::critical		(std::string);
	D::error		(std::string);
	
	// float
	D::info			(float);
	D::debug		(float);
	D::warning		(float);
	D::critical		(float);
	D::error		(float);
	
	// int
	D::info			(int);
	D::debug		(int);
	D::warning		(int);
	D::critical		(int);
	D::error		(int);
	
	// int
	D::info			(char);
	D::debug		(char);
	D::warning		(char);
	D::critical		(char);
	D::error		(char);
	
	// int
	D::info			(double);
	D::debug		(double);
	D::warning		(double);
	D::critical		(double);
	D::error		(double);

}