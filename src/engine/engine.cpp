#include "engine.h"


int _argc;
char** _argv;
int oe::argc() {
	return _argc;
}

char** oe::argv() {
	return _argv;
}

void oe::init(int _argc_, char* _argv_[]) {
	_argc = _argc_;
	_argv = _argv_;

	spdlog::set_pattern("%^[%T] [%l]:%$ %v");
	spdlog::set_level(spdlog::level::level_enum::trace);

	spdlog::info("Engine initialized");
}

void oe::terminate() {
#ifdef _DEBUG
	spdlog::info("Press <ENTER> to exit");
	assert(0);
#else
#endif // _DEBUG
}

void oe::__error(std::string error_msg, int line, std::string file) {
	spdlog::error("error: {}\nline: {}\nfile: {}", error_msg, line, file);
	oe::terminate();
}