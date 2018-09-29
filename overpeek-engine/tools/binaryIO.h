#pragma once


#include <string>

namespace tools {

	class BinaryIO {
	public:
		static std::string filePath;
		static void write(std::string name, unsigned char *data, size_t dataSize);
		static unsigned char* read(std::string name);

	};

}