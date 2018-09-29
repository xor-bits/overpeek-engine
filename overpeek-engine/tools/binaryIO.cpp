#include "binaryIO.h"

#include <cstdio>
#include <iostream>

namespace tools {
	
	std::string BinaryIO::filePath;

	void BinaryIO::write(std::string name, unsigned char *data, size_t dataSize) {
		FILE *pFile = fopen((filePath + name).c_str(), "wb");
		if (pFile == NULL) { fputs("ERROR Opening file!", stderr); system("pause");  exit(1); }
		fwrite(data, sizeof(unsigned char), dataSize, pFile);
		fclose(pFile);
	}
	
	unsigned char *BinaryIO::read(std::string name) {
		FILE *pFile;
		long lSize;
		unsigned char *buffer;
		size_t result;

		pFile = fopen((filePath + name).c_str(), "rb");
		if (pFile == NULL) return nullptr;

		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		buffer = (unsigned char*)malloc(sizeof(unsigned char)*lSize);
		if (buffer == NULL) return nullptr;

		result = fread(buffer, 1, lSize, pFile);
		if (result != lSize) return nullptr;

		fclose(pFile);
		return buffer;
	}
}