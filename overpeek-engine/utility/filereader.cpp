#include "filereader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "../internal_libs.h"

namespace oe {

	std::string readFile(std::string filepath)
	{
		std::ifstream input_file_stream = std::ifstream(filepath);
		std::stringstream buffer;
		buffer << input_file_stream.rdbuf();
		return buffer.str();
	}

}