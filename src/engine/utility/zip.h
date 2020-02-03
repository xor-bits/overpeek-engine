#pragma once

#include <cstdint>



namespace oe::utils {

	// input is compressed to output
	// output needs to be nullptr
	uint8_t* compress(uint64_t& output_size, const uint8_t* input, uint64_t input_size);

	// input is uncompressed to output
	// output needs to be nullptr
	uint8_t* uncompress(uint64_t& output_size, const uint8_t* input, uint64_t input_size);

}