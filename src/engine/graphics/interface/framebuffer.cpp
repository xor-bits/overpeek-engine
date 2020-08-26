#include "framebuffer.hpp"

#include "primitive_renderer.hpp"
#include "window.hpp"



namespace oe::graphics {

	IFrameBuffer::IFrameBuffer(const FrameBufferInfo& framebuffer_info, const Window& window)
		: m_framebuffer_info(framebuffer_info) 
	{

	}
	
	IFrameBuffer::~IFrameBuffer() 
	{

	}

	void IFrameBuffer::multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, const Window& window, const std::array<VertexData, 4>& vertices, size_t count)
	{
		FramebufferMultipass::getSingleton().multipass(fb_0, fb_1, window, vertices, count);
	}

	FramebufferMultipass* FramebufferMultipass::fb_multipasser = nullptr;
	FramebufferMultipass::FramebufferMultipass()
	{
		RendererInfo ri = {};
		ri.max_primitive_count = 1;
		m_primitive_renderer = new PrimitiveRenderer(ri);
	}

	void FramebufferMultipass::multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, const Window& window, const std::array<VertexData, 4>& vertices, size_t count)
	{
		PrimitiveRenderer& pr = *reinterpret_cast<PrimitiveRenderer*>(m_primitive_renderer);
		pr->begin();
		pr->clear();
		pr->submitVertex(vertices.data(), vertices.size(), 0);
		pr->end();

		for (int i = 0; i < count; i++) {
			fb_1->bind();
			fb_1->clear();
			fb_0->getTexture()->bind();
			pr->render(1);
			
			fb_0->bind();
			fb_0->clear();
			fb_1->getTexture()->bind();
			pr->render(1);
		}
		window->bind();
		
	}

}
