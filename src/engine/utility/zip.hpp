#pragma once

#include <cstdint>
#include <vector>
#include <string>



namespace oe::utils {

	class Compression {
	private:
		static Compression* singleton;
		Compression() {}

	public:
		Compression(const Compression&) = delete;
		static Compression& getSingleton() {
			if (!singleton) singleton = new Compression();
			return *singleton;
		}

	public:
		// input is compressed to output
		std::vector<uint8_t> compress(const std::vector<uint8_t>& input);

		// input is compressed to output
		std::vector<uint8_t> compress(const unsigned char* data, size_t size);

		// input is uncompressed to output
		std::vector<uint8_t> uncompress(const std::vector<uint8_t>& input);

		// input is uncompressed to output
		std::vector<uint8_t> uncompress(const unsigned char* data, size_t size);

		std::string data_to_string(const std::vector<uint8_t>& input);
		std::vector<uint8_t> string_to_data(const std::string& input);
	};

}