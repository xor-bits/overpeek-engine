#pragma once

#include <string>


namespace oe {

	std::string readFile(const char *filepath);

	std::string readSetting(const char *filepath, const char *location);

	void writeSetting(const char *filepath, const char *location, std::string value);

}