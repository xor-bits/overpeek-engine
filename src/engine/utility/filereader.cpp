#include "filereader.hpp"

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

std::string oe::utils::readFile(fs::path path)
{
	std::ifstream input_file_stream = std::ifstream(path);
	if (!input_file_stream.is_open()) {
		oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
	}

	std::stringstream buffer;
	buffer << input_file_stream.rdbuf();
	return buffer.str();
}

void oe::utils::saveImage(fs::path path, const oe::utils::image_data& image)
{
	int channels = stb_i_channels(image.format);
	stbi_write_png(path.string().c_str(), image.width, image.height, channels, image.data, image.width * channels);
}

const oe::utils::image_data oe::utils::loadImage(fs::path path, oe::formats format) {
	int width, height, channels;
	unsigned char* image = stbi_load(path.string().c_str(), &width, &height, &channels, stb_i_format(format));

	if (!image) {
		oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
	}

	// return
	image_data returned = {};
	returned.format = format;
	returned.size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
	returned.data = image;
	returned.width = width;
	returned.height = height;

	return returned;
}

const oe::utils::image_data oe::utils::loadImageCopy(const unsigned char* data, int width, int height, oe::formats format) {
	size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(4);
	unsigned char* data_dest = new unsigned char[size];
	std::memcpy(data_dest, data, size);

	// return
	image_data returned = {};
	returned.size = size;
	returned.data = data_dest;
	returned.width = width;
	returned.height = height;
	returned.format = format;

	return returned;
}

const oe::utils::image_data oe::utils::loadImageMove(unsigned char* data, int width, int height, oe::formats format) {
	// return
	image_data returned = {};
	returned.size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(4);
	returned.data = data;
	returned.width = width;
	returned.height = height;
	returned.format = format;

	return returned;
}

const oe::utils::audio_data oe::utils::loadAudio(fs::path path) {
	mp3dec_t mp3d;
	mp3dec_file_info_t info;
	if (mp3dec_load(&mp3d, path.string().c_str(), &info, NULL, NULL)) {
		oe_error_terminate("Failed to load audiofile \"{}\"", std::string(path.string().c_str()));
	}

	// data size
	size_t size = info.samples * sizeof(int16_t);

	// Format
	int format = -1;
	if (info.channels == 1) {
		format = AL_FORMAT_MONO16;
	}
	else if (info.channels == 2) {
		format = AL_FORMAT_STEREO16;
	}

	// return
	audio_data returned = {};
	returned.data = info.buffer;
	returned.channels = info.channels;
	returned.sample_rate = info.hz;
	returned.format = format;
	returned.size = size;

	return returned;
}

void oe::utils::freeImage(oe::utils::image_data& image) {
	delete[] image.data;
}

void oe::utils::freeAudio(oe::utils::audio_data& audio) {
	delete[] audio.data;
}