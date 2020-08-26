#pragma once

#include "buffer.hpp"
#include "engine/enum.hpp"



namespace oe::graphics
{
	class StorageBuffer
	{
	private:
		unsigned int p_id;
		size_t p_size;
		int p_target;

	public:
		StorageBuffer(const void* data, size_t size, oe::types buffer_type);

		void bind() const;
		void unbind() const;
		void compute(size_t binding) const;

		void* mapBuffer();
		void unmapBuffer();
		void setBufferData(const void* data, size_t size);

		inline size_t getSize() const { return p_size; }
		inline size_t getId() const { return p_id; }
		inline size_t getTarget() const { return p_target; }

	};
}