#include "framebuffer.hpp"

#include "renderer.hpp"



namespace oe::graphics {

	FrameBuffer::FrameBuffer(const FrameBufferInfo& framebuffer_info, Window* window)
		: m_framebuffer_info(framebuffer_info) 
	{

	}
	
	FrameBuffer::~FrameBuffer() 
	{

	}

}
