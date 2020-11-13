#include "buffer.hpp"

#include "engine/graphics/opengl/gl_include.hpp"

#include <iomanip>
#include <sstream>
#include <cstddef>

#include "engine/engine.hpp"


namespace oe::graphics {

	Buffer::Buffer(const void* data, int32_t size, int target, unsigned int usage)
		: p_size(std::abs(size))
		, p_target(target)
		, p_mapped(nullptr)
	{
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
		if(p_mapped != nullptr)
			return p_mapped;

		bind();
		p_mapped = glMapBuffer(p_target, GL_WRITE_ONLY);
		return p_mapped;
	}

	void Buffer::unmapBuffer()
	{
		if (p_mapped == nullptr)
			return;
		
		bind();
		glUnmapBuffer(p_target);
		p_mapped = nullptr;
	}

	void Buffer::setBufferData(const void *data, int32_t size)
	{
		bind();
		glBufferSubData(p_target, 0, std::abs(size), data);
	}
}
