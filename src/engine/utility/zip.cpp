#include "zip.hpp"

#include "engine/internal_libs.hpp"
#include "engine/engine.hpp"



namespace oe::utils {

	uint8_t* compress(uint64_t& output_size, const uint8_t* input, uint64_t input_size) {
		mz_ulong uncompressedSize = static_cast<mz_ulong>(input_size);
		const uint8_t* uncompressedData = input;
		mz_ulong compressedSize = mz_compressBound(uncompressedSize);
		uint8_t* compressedData = new uint8_t[compressedSize + sizeof(uint64_t)]; // additional 8 bytes at the end indicating original size
		int state = mz_compress(compressedData, &compressedSize, uncompressedData, uncompressedSize);
		if (state != MZ_OK) oe_error_terminate("Error compressing data");

		// additional 8 bytes at the end indicating original size
		*reinterpret_cast<uint64_t*>(compressedData + compressedSize) = input_size;

		output_size = static_cast<size_t>(compressedSize);
		return compressedData;
	}

	uint8_t* uncompress(uint64_t& output_size, const uint8_t* input, uint64_t input_size) {
		// additional 8 bytes at the end indicating original size
		output_size = *reinterpret_cast<const uint64_t*>(input + input_size);

		mz_ulong compressedSize = static_cast<mz_ulong>(input_size);
		const uint8_t* compressedData = input;
		mz_ulong uncompressedSize = output_size;
		uint8_t* uncompressedData = new uint8_t[compressedSize];
		int state = mz_uncompress(uncompressedData, &uncompressedSize, compressedData, compressedSize);
		if (state != MZ_OK) oe_error_terminate("Error compressing data");

		return uncompressedData;
	}

}