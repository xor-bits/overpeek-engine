#include "filereader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "internal_libs.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <stb_vorbis.c>



namespace oe::utils {

	std::string readFile(std::filesystem::path path)
	{
		std::ifstream input_file_stream = std::ifstream(path);
		std::stringstream buffer;
		buffer << input_file_stream.rdbuf();
		return buffer.str();
	}

	void saveImage(std::filesystem::path path, const image_data& image) {
		stbi_write_png(path.string().c_str(), image.width, image.height, image.channels, image.data, image.width * image.channels * sizeof(char));
	}

	const image_data loadImage(std::filesystem::path path) {
		int width, height, channels;
		unsigned char* image = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		
		if (!image) {
			spdlog::error("Failed to load imagefile \"{}\"", std::string(path.string().c_str()));
			return image_data();
		}

		return image_data(image, width, height, channels);
	}

	void freeImage(const image_data& image) {
		delete[] image.data;
	}

}