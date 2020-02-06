#pragma once

#include <string>
#include <fstream>



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
		const unsigned char* data;
		int width, height, channels;
		size_t size;
	};

	struct audio_data {
		const short* data;
		int format, size, channels, sample_rate;
	};

	// Read contents of file to string
	std::string readFile(fs::path path);

	// Should save image to file
	// Not tested
	void saveImage(fs::path path, const image_data& image);

	const image_data loadImage(fs::path path);
	const audio_data loadAudio(fs::path path);
	const image_data loadImageCopy(const unsigned char* data, int width, int height); // data is copied to image_data // can be freed and used with spritepack
	const image_data loadImageMove(unsigned char* data, int width, int height);        // image_data will use data // do not free if used with spritepack
	void freeImage(image_data& img);
	void freeAudio(audio_data& aud);

	// data size is count of T elements to write
	template<class T> static void write(std::string name, const T* data, size_t data_size) {
		std::ofstream output_stream(name);
		output_stream.write((const char*)data, data_size * sizeof(T));
		output_stream.close();
	}

	// data size is count of T elements to read
	template<class T> static void read(std::string name, T* data, size_t& data_size) {
		std::ifstream input_stream(name);
		input_stream.seekg(input_stream.end);
		data_size = input_stream.tellg() / sizeof(T);
		data = new T[data_size];
		input_stream.read((char*)data, data_size * sizeof(T));
		input_stream.close();
	}

}
