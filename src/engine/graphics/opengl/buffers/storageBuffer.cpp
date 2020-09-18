#include "storageBuffer.hpp"

#include "engine/graphics/opengl/gl_include.hpp"



namespace oe::graphics
{
	StorageBuffer::StorageBuffer(const void* data, int32_t size, oe::types buffer_type)
		: p_size(std::abs(size))
		, p_target(GL_SHADER_STORAGE_BUFFER)
    {
        GLenum usage = buffer_type == oe::types::dynamic_type ? GL_DYNAMIC_COPY : GL_STATIC_COPY;

        glGenBuffers(1, &p_id);
        glBindBuffer(p_target, p_id);
        glBufferData(p_target, p_size, data, usage);
        glBindBufferBase(p_target, 3, p_id);
        glBindBuffer(p_target, 0); // unbind
    }

    void StorageBuffer::bind() const
    {
        glBindBuffer(p_target, p_id);
    }

    void StorageBuffer::unbind() const
    {
        glBindBuffer(p_target, 0);
    }

    void StorageBuffer::compute(int32_t binding) const
    {
        bind();
        glBindBufferBase(p_target, binding, p_id);
    }

    void* StorageBuffer::mapBuffer()
    {
		bind();
		return glMapBuffer(p_target, GL_WRITE_ONLY);
    }

    void StorageBuffer::unmapBuffer()
    {
		bind();
        glUnmapBuffer(p_target);
    }

    void StorageBuffer::setBufferData(const void* data, int32_t size)
    {
		bind();
		glBufferSubData(p_target, 0, std::abs(size), data);
    }

}