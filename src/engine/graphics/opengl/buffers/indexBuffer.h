#pragma once

#include "buffer.h"



namespace oe::graphics {

	class IndexBuffer : public Buffer {
	public:
		IndexBuffer(unsigned short int *data, size_t size, int buffer_type);
	};

}