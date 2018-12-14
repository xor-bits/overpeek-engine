#pragma once

#include <string>
#include <fstream>

#include "logger.h"

namespace tools {

	std::string readFile(const char *filepath)
	{
		FILE* file = fopen(filepath, "rt");
		if (!file) {
			tools::Logger::error(std::string("Couldn't open file ") + filepath);
		}
		fseek(file, 0, SEEK_END);
		unsigned long length = ftell(file);
		char *data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, length, file);
		fclose(file);

		std::string result(data);
		delete[] data;
		return result;
	}

}