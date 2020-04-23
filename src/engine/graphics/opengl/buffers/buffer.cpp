#include "buffer.hpp"

#include <glad/glad.h>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "engine/engine.hpp"


namespace oe::graphics {

	Buffer::Buffer(const void* data, size_t size, int target, unsigned int usage) 
	{
		oe_debug_call("gl_buffer");

		p_target = target;
		p_size = size;

		glGenBuffers(1, &p_id);
		bind();

		glBufferData(target, size, data, usage);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &p_id);
	}

	void Buffer::bind() {
		glBindBuffer(p_target, p_id);
	}

	void Buffer::unbind() {
		glBindBuffer(p_target, 0);
	}

	void* Buffer::mapBuffer() {
		bind();
		void* buffer = glMapBuffer(p_target, GL_WRITE_ONLY);
		return buffer;
	}

	void Buffer::unmapBuffer() {
		bind();
		glUnmapBuffer(p_target);
	}

	void Buffer::setBufferData(const void *data, size_t size) {
		bind();
		glBufferSubData(p_target, 0, size, data);
	}
}
