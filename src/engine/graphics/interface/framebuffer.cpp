#include "framebuffer.hpp"

#include "engine/interfacegen_renderer.hpp"
#include "texture.hpp"
#include "primitive_renderer.hpp"
#include "window.hpp"



namespace oe::graphics
{
	class FramebufferMultipass
	{
	private:
		FramebufferMultipass()
		{
			RendererInfo ri = {};
			ri.max_primitive_count = 1;
			m_primitive_renderer = new PrimitiveRenderer(ri);
		}
		static FramebufferMultipass* fb_multipasser;

	public:
		static FramebufferMultipass& getSingleton()
		{
			if (!fb_multipasser) fb_multipasser = new FramebufferMultipass();
			return *fb_multipasser;
		}

		void multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, const std::array<VertexData, 4>& vertices, size_t count)
		{
			PrimitiveRenderer& pr = *reinterpret_cast<PrimitiveRenderer*>(m_primitive_renderer);
			pr->begin();
			pr->clear();
			pr->submitVertex(vertices, 0);
			pr->end();

			for (size_t i = 0; i < count; i++) {
				fb_1->bind();
				fb_1->clear();
				fb_0->getTexture()->bind();
				pr->render(1);
				
				fb_0->bind();
				fb_0->clear();
				fb_1->getTexture()->bind();
				pr->render(1);
			}
		}

	private:
		void* m_primitive_renderer;

	};
	FramebufferMultipass* FramebufferMultipass::fb_multipasser = nullptr;



	IFrameBuffer::IFrameBuffer(const FrameBufferInfo& framebuffer_info)
		: m_framebuffer_info(framebuffer_info) 
	{

	}
	
	IFrameBuffer::~IFrameBuffer() 
	{

	}

	void IFrameBuffer::multipass(FrameBuffer& fb_0, FrameBuffer& fb_1, const std::array<VertexData, 4>& vertices, size_t count)
	{
		FramebufferMultipass::getSingleton().multipass(fb_0, fb_1, vertices, count);
	}

}
