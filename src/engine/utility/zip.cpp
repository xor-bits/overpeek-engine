#include "zip.hpp"

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"

#include <sstream>
#include <array>



namespace oe::utils {

	Compression* Compression::singleton = nullptr;

	std::vector<uint8_t> Compression::compress(const std::vector<uint8_t>& input) {
		return compress(input.data(), input.size());
	}

	std::vector<uint8_t> Compression::compress(const unsigned char* data, size_t size) {
		mz_ulong compressedSize = mz_compressBound(size);
		std::vector<uint8_t> compressedData;
		compressedData.resize(compressedSize);

		// compress to vector
		int state = mz_compress(compressedData.data(), &compressedSize, data, size);
		if (state != MZ_OK) oe_error_terminate("Error compressing data");

		// shrink the vector
		compressedData.resize(compressedSize);

		return compressedData;
	}

	std::vector<uint8_t> Compression::uncompress(const std::vector<uint8_t>& input) {
		return uncompress(input.data(), input.size());
	}

	std::vector<uint8_t> Compression::uncompress(const unsigned char* data, size_t size) {
		mz_stream stream = {};
		mz_inflateInit(&stream);

		std::vector<uint8_t> output_stream;

		stream.avail_in = size;
		stream.next_in = data;

		while (true) {
			const uint64_t output_buffer_size = 512;
			uint8_t* output_buffer = new uint8_t[output_buffer_size]();

			stream.avail_out = output_buffer_size;
			stream.next_out = output_buffer;

			int result = mz_inflate(&stream, MZ_NO_FLUSH);
			if (result < 0) oe_error_terminate("mz_inflate() failed");
			else if (result == MZ_STREAM_END) {
				result = mz_inflateEnd(&stream);
				if (result < 0) oe_error_terminate("mz_inflateEnd() failed");

				size_t size = stream.total_out % output_buffer_size;
				output_stream.insert(output_stream.end(), output_buffer, output_buffer + size);

				// decompression complete
				break;
			}

			size_t size = output_buffer_size;
			output_stream.insert(output_stream.end(), output_buffer, output_buffer + size);
			// continue
		}

		return output_stream;
	}

	std::string Compression::data_to_string(const std::vector<uint8_t>& input) {
		return std::string(input.begin(), input.end());
	}

	std::vector<uint8_t> Compression::string_to_data(const std::string& input) {
		return std::vector<uint8_t>(input.begin(), input.end());
	}
	
}