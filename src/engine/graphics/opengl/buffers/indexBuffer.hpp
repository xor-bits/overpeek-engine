#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"



namespace oe::graphics {

	class IndexBuffer : public Buffer {
	public:
		IndexBuffer(unsigned short int *data, size_t size, oe::types buffer_type);
	};

}