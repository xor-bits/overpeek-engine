#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"



namespace oe::graphics
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(const void *data, int32_t size, oe::types buffer_type);
	};
}