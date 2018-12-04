#pragma once


#include <string>

namespace tools {

	class BinaryIO {
	public:
		static std::string filePath;
		static void write(std::string name, void* data, size_t dataSize);
		static void* read(std::string name);

	};

}