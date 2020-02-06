#include "renderer.h"



namespace oe::graphics {

	Renderer::Renderer(const Instance* instance, const RendererInfo& renderer_info)
	{
		m_max_quad_count = renderer_info.max_quad_count;
		m_quad_count = 0;
	}

	Renderer::~Renderer() {}

}
