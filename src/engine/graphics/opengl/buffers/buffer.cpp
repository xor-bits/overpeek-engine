#include "buffer.hpp"

#include "engine/graphics/opengl/gl_include.hpp"

#include <iomanip>
#include <sstream>
#include <cstddef>
#include <spdlog/spdlog.h>

#include "engine/engine.hpp"


namespace oe::graphics {

	Buffer::Buffer(const void* data, int32_t size, int target, unsigned int usage)
		: p_size(std::abs(size))
		, p_target(target)
		, p_mapped(false)
	{
		oe_debug_call("gl_buffer");

		glGenBuffers(1, &p_id);
		glBindBuffer(p_target, p_id);

		glBufferData(target, p_size, data, usage);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &p_id);
	}

	void Buffer::bind()
	{
		glBindBuffer(p_target, p_id);
	}

	void Buffer::unbind()
	{
		glBindBuffer(p_target, 0);
	}

	void* Buffer::mapBuffer()
	{
		bind();
		p_mapped = true;
		return glMapBuffer(p_target, GL_WRITE_ONLY);
	}

	void Buffer::unmapBuffer()
	{
		if (!p_mapped)
		{
			spdlog::warn("buffer was not mapped");
			return;
		}
		
		bind();
		glUnmapBuffer(p_target);
		p_mapped = false;
	}

	void Buffer::setBufferData(const void *data, int32_t size)
	{
		bind();
		glBufferSubData(p_target, 0, std::abs(size), data);
	}
}
