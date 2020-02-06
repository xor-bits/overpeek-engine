#pragma once

#include "buffer.h"
#include "engine/enum.h"



namespace oe::graphics {

	class IndexBuffer : public Buffer {
	public:
		IndexBuffer(unsigned short int *data, size_t size, oe::types buffer_type);
	};

}