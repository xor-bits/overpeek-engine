#include "filereader.h"


#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>

#include "logger.h"

///TODO: Performance improvements for writeSetting()

namespace oe {

	std::string readFile(const char *filepath)
	{
		FILE* file = fopen(filepath, "rt");
		if (!file) {
			oe::Logger::out("Couldn't open file ", filepath, oe::error);
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

	std::string readSetting(const char *filepath, const char *location)
	{
		std::ifstream file(filepath);
		std::string str;
		std::string result;
		
		
		while (std::getline(file, str))
		{
			int equals = str.find(" = ");
		
			if (std::string(location) == str.substr(0, equals)) {
				result = str.substr(equals + 3);
				file.close();
				return result;
			}
		}
		file.close();
		return "0";
	}

	void writeSetting(const char *filepath, const char *location, std::string value) {
		std::ifstream infile(filepath);
		std::string output;

		std::string str;
		while (std::getline(infile, str))
		{
			int equals = str.find(" = ");
			if (str.substr(0, equals) == location) {
				str = std::string(location) + " = " + value;
			}
			str += "\n";
			output.append(str);
		}
		infile.close();

		std::ofstream outfile(filepath);
		outfile.write(output.c_str(), output.size());
		outfile.close();
	}

}