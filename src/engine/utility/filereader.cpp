#include "filereader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "engine/internal_libs.h"
#include "engine/engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define MINIMP3_IMPLEMENTATION
// #include "minimp3.h"
#include "minimp3_ex.h"

#include <al.h>



namespace oe::utils {

	std::string readFile(fs::path path)
	{
		std::ifstream input_file_stream = std::ifstream(path);
		std::stringstream buffer;
		buffer << input_file_stream.rdbuf();
		return buffer.str();
	}

	void saveImage(fs::path path, const image_data& image)
	{
		stbi_write_png(path.string().c_str(), image.width, image.height, image.channels, image.data, image.width * image.channels * sizeof(char));
	}

	const image_data loadImage(fs::path path) {
		int width, height, channels;
		unsigned char* image = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		
		if (!image) {
			oe_error_terminate("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
		}

		// return
		image_data returned = {};
		returned.size = width * height * channels;
		returned.data = image;
		returned.width = width;
		returned.height = height;
		returned.channels = channels;

		return returned;
	}

	const image_data loadImageCopy(const unsigned char* data, int width, int height) {
		size_t size = width * height * 4;
		unsigned char* data_dest = new unsigned char[size];
		std::memcpy(data_dest, data, size);

		// return
		image_data returned = {};
		returned.size = size;
		returned.data = data_dest;
		returned.width = width;
		returned.height = height;
		returned.channels = 4;

		return returned;
	}

	const image_data loadImageMove(unsigned char* data, int width, int height) {
		// return
		image_data returned = {};
		returned.size = width * height * 4;
		returned.data = data;
		returned.width = width;
		returned.height = height;
		returned.channels = 4;

		return returned;
	}

	const audio_data loadAudio(fs::path path) {
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

	void freeImage(image_data& image) {
		delete[] image.data;
	}

	void freeAudio(audio_data& audio) {
		delete[] audio.data;
	}

}
