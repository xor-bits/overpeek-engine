#pragma once


#include <cstdio>
#include <iostream>
#include <string>

namespace tools {

	class BinaryIO {
	public:
		template<typename T> static void write(std::string name, void* data, size_t dataSize) {
			FILE *pFile = fopen(name.c_str(), "wb");
			if (pFile == NULL) { fputs("ERROR Opening file!", stderr); system("pause");  exit(1); }
			fwrite(data, sizeof(T), dataSize, pFile);
			fclose(pFile);
		}

		template<typename T> static void* read(std::string name) {
			FILE *pFile;
			long lSize;
			T *buffer;
			size_t result;

			pFile = fopen(name.c_str(), "rb");
			if (pFile == NULL) return nullptr;

			fseek(pFile, 0, SEEK_END);
			lSize = ftell(pFile);
			rewind(pFile);

			buffer = (T*)malloc(sizeof(T)*lSize);
			if (buffer == NULL) return nullptr;

			result = fread(buffer, 1, lSize, pFile);
			if (result != lSize) return nullptr;

			fclose(pFile);
			return buffer;
		}
	};

}