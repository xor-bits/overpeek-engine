#pragma once

#include <cstddef>
#include <cstdint>

namespace oe::graphics
{
	class Buffer
	{
	protected:
		unsigned int p_id;
		int32_t p_size;
		int p_target;
		bool p_mapped;

		// int32_t size (in bytes)
		Buffer(const void* data, int32_t size, int target, unsigned int usage);

	public:
		virtual ~Buffer();
		
		// Binding
		virtual void bind();
		virtual void unbind();
		
		// Data
		virtual void* mapBuffer();
		virtual void unmapBuffer();
		virtual void setBufferData(const void* data, int32_t size);

		// Getters/setters
		inline int32_t getSize() const { return p_size; }
		inline int32_t getId() const { return p_id; }
		inline int32_t getTarget() const { return p_target; }
	};
}
