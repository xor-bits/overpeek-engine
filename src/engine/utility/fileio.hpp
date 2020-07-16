#pragma once

#include <string>
#include <fstream>

#include "engine/enum.hpp"



#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#  if __has_include(<filesystem>)
#    include <filesystem>
namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#  endif
#endif



namespace oe::utils {

	struct image_data {
		uint8_t* data;
		oe::formats format;
		int width, height;
		size_t size;

		image_data(fs::path path, oe::formats format = oe::formats::rgba);
		image_data(const uint8_t* data, oe::formats format, int width, int height);
		image_data(const image_data& copied);
		~image_data();
	};

	struct audio_data {
		int16_t* data;
		int format, size, channels, sample_rate;

		audio_data(fs::path path);
		audio_data(const int16_t* data, int format, int size, int channels, int sample_rate);
		audio_data(const audio_data& copied);
		~audio_data();
	};

	class FileIO {
	private:
		static FileIO* singleton;
		FileIO();

	public:
		FileIO(const FileIO&) = delete;
		static FileIO& getSingleton() {
			if (!singleton) singleton = new FileIO();
			return *singleton;
		}

	public:
		bool fileExists(fs::path path);

		// save image to file
		void saveImage(fs::path path, const image_data& image);

		// image loading
		image_data loadImage(fs::path path, oe::formats format = oe::formats::rgba);
		image_data loadImage(const unsigned char* data, int width, int height, oe::formats format = oe::formats::rgba);

		// audio loading
		audio_data loadAudio(fs::path path);

		// read contents of a file to string
		void readString(std::string& string, fs::path path);
		void writeString(const std::string& string, fs::path path);

		// data size is count of T elements to write
		template<typename T> static void write(std::string name, const T* data, size_t data_size) {
			std::ofstream output_stream(name);
			output_stream.write((const char*)data, data_size * sizeof(T));
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
