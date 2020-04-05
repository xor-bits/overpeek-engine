#include "renderer.hpp"



namespace oe::graphics {

	Renderer::Renderer(const RendererInfo& renderer_info)
		: m_renderer_info(renderer_info)
	{
		m_quad_count = 0;
	}

	Renderer::~Renderer() {}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color, glm::vec2 align, float angle, int quad_index) {
		submit(glm::vec3(position, 0.0f), size, sprite, color, align, angle, quad_index);
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, glm::vec2 align, float angle, int quad_index) {
		submit(glm::vec3(position, 0.0f), size, color, align, angle, quad_index);
	}

}
