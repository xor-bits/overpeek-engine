#pragma once


#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include "../internal_libs.h"

namespace oe {

	class BinaryIO {
	public:

		// data size is count of T elements to write
		template<typename T> static void write(std::string name, const T* data, size_t data_size) {
			std::ofstream output_stream(name);
			output_stream.write((const char *)data, data_size * sizeof(T));
			output_stream.close();
		}

		// data size is count of T elements to read
		template<typename T> static void read(std::string name, T* data, size_t& data_size) {
			std::ifstream input_stream(name);
			input_stream.seekg(input_stream.end);
			data_size = input_stream.tellg() / sizeof(T);
			data = new T[data_size];
			input_stream.read((char*)data, data_size * sizeof(T));
			input_stream.close();
		}
	};

}