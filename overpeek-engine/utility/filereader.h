#pragma once

#include <string>
#include <filesystem>



namespace oe {

	struct image_data {
		unsigned char* data;
		size_t width, height, channels;
	};

	struct audio_data {
		short* data;
		size_t size;
		size_t format, channels, sample_rate;
	};

	// Read contents of file to string
	std::string readFile(std::filesystem::path path);

	// Should save image to file
	// Not tested
	void saveImage(std::filesystem::path path, const image_data& image);

	// Load image_data from file
	// Remember to freeImage() after use
	const image_data loadImage(std::filesystem::path path);

	// Load audio_data from file
	// Remember to freeAudio() after use
	const audio_data loadAudio(std::filesystem::path path);

	// Free image data
	void freeImage(const image_data& image);
	
	// Free audio data
	void freeAudio(const audio_data& audio);

}