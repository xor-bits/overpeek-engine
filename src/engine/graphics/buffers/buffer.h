#pragma once

#include <cstddef>

namespace oe::graphics {

	class Buffer {
	protected:
		unsigned int p_id;
		size_t p_size;
		int p_target;

		// size_t size (in bytes)
		Buffer(const void* data, size_t size, int target, unsigned int usage);

	public:
		~Buffer();
		
		// Binding

		virtual void bind();
		virtual void unbind();

		
		
		// Data

		virtual void* mapBuffer();
		virtual void unmapBuffer();
		virtual void setBufferData(const void* data, size_t size);



		// Getters/setters
		
		inline size_t getSize() const { return p_size; }
		inline size_t getId() const { return p_id; }
		inline size_t getTarget() const { return p_target; }
	};

}
