#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"



namespace oe::graphics
{
	class StorageBuffer
	{
	private:
		unsigned int p_id;
		int32_t p_size;
		int p_target;

	public:
		StorageBuffer(const void* data, int32_t size, oe::types buffer_type);

		void bind() const;
		void unbind() const;
		void compute(int32_t binding) const;

		void* mapBuffer();
		void unmapBuffer();
		void setBufferData(const void* data, int32_t size);

		inline int32_t getSize() const { return p_size; }
		inline int32_t getId() const { return p_id; }
		inline int32_t getTarget() const { return p_target; }

	};
}