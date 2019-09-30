#pragma once

#include "buffer.h"

namespace oe {

	class IndexBuffer : public Buffer {
	public:
		IndexBuffer(unsigned short int *data, size_t size, int buffer_type);
	};

}