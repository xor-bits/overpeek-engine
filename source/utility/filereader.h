#pragma once

#include <string>
#include <filesystem>



namespace oe::utils {

	struct image_data {
		unsigned char* data;
		int width, height, channels;

		image_data(unsigned char* _data, int _width, int _height, int _channels) {
			data = _data;
			width = _width;
			height = _height;
			channels = _channels;
		}

		image_data() : image_data(nullptr, 0, 0, 0) {}
	};

	// Read contents of file to string
	std::string readFile(std::filesystem::path path);

	// Should save image to file
	// Not tested
	void saveImage(std::filesystem::path path, const image_data& image);

	// Load image_data from file
	// Remember to freeImage() after use
	const image_data loadImage(std::filesystem::path path);

	// Free image data
	void freeImage(const image_data& image);

}