#include "fileio.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"

#include <al.h>



int stb_i_format(oe::formats format) {
	switch (format)
	{
	case oe::formats::rgba:
		return STBI_rgb_alpha;
	case oe::formats::rgb:
		return STBI_rgb;
	case oe::formats::mono:
		return STBI_grey;
	}
}

int stb_i_channels(oe::formats format) {
	switch (format)
	{
	case oe::formats::rgba:
		return 4;
	case oe::formats::rgb:
		return 3;
	case oe::formats::mono:
		return 1;
	}
}



namespace oe::utils {

	image_data::image_data(fs::path path, oe::formats _format) {
		int channels;
		data = stbi_load(path.string().c_str(), &width, &height, &channels, stb_i_format(_format));

		if (!data) {
			oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
		}

		format = _format;
		size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
	}

	image_data::image_data(const uint8_t* _data, oe::formats _format, int _width, int _height)
		: format(_format)
		, width(_width), height(_height)
		, size(_width* _height* stb_i_channels(_format))
	{
		data = new uint8_t[size];
		std::memcpy(data, _data, size);
	}

	image_data::image_data(const image_data& _copied)
		: format(_copied.format)
		, width(_copied.width), height(_copied.height)
		, size(_copied.width* _copied.height* stb_i_channels(_copied.format))
	{
		data = new uint8_t[size];
		std::memcpy(data, _copied.data, size);
	}

	image_data::~image_data() {
		delete data;
	}

	audio_data::audio_data(fs::path path) {
		mp3dec_t mp3d;
		mp3dec_file_info_t info;
		if (mp3dec_load(&mp3d, path.string().c_str(), &info, NULL, NULL)) {
			oe_error_terminate("Failed to load audiofile \"{}\"", std::string(path.string().c_str()));
		}

		size = info.samples * sizeof(int16_t);
		data = info.buffer;
		channels = info.channels;
		sample_rate = info.hz;

		// Format
		format = -1;
		if (info.channels == 1) {
			format = AL_FORMAT_MONO16;
		}
		else if (info.channels == 2) {
			format = AL_FORMAT_STEREO16;
		}
	}

	audio_data::audio_data(const int16_t* _data, int _format, int _size, int _channels, int _sample_rate)
		: format(_format)
		, size(_size)
		, channels(_channels)
		, sample_rate(_sample_rate)
	{
		data = new int16_t[size];
		std::memcpy(data, _data, size);
	}

	audio_data::audio_data(const audio_data& _copied)
		: format(_copied.format)
		, size(_copied.size)
		, channels(_copied.channels)
		, sample_rate(_copied.sample_rate)
	{
		data = new int16_t[size];
		std::memcpy(data, _copied.data, size);
	}

	audio_data::~audio_data() {
		delete data;
	}

}

namespace oe::utils { // just to make the code look prettier

	FileIO* FileIO::singleton = nullptr;

	FileIO::FileIO() {
		utils::Clock::getSingleton(); // init to start the timer
	}



	void FileIO::loadString(std::string& string, fs::path path)
	{
		std::ifstream input_file_stream = std::ifstream(path);
		if (!input_file_stream.is_open()) {
			oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
		}

		std::stringstream buffer;
		buffer << input_file_stream.rdbuf();
		string = buffer.str();
	}

	void FileIO::saveImage(fs::path path, const image_data& image)
	{
		int channels = stb_i_channels(image.format);
		stbi_write_png(path.string().c_str(), image.width, image.height, channels, image.data, image.width * channels);
	}

	image_data FileIO::loadImage(fs::path path, oe::formats format) {
		return image_data(path, format);
	}

	audio_data loadAudio(fs::path path) {
		return audio_data(path);
	}

}