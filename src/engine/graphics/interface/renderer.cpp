#include "renderer.hpp"

#include "engine/utility/extra.hpp"



namespace oe::graphics {

	Renderer::Renderer(const RendererInfo& renderer_info)
		: m_renderer_info(renderer_info)
		, m_vertex_count(0)
		, m_buffer_pos(0)
	{
	}

	Renderer::~Renderer() {}

	void gen_points(const glm::vec2& position, const glm::vec2& size, const glm::vec2& align, float angle, glm::vec2& pointA, glm::vec2& pointB, glm::vec2& pointC, glm::vec2& pointD) {
		glm::vec2 alignment = alignmentOffset(size, align);
		pointA = size * glm::vec2(0.0f, 0.0f) - alignment;
		pointB = size * glm::vec2(0.0f, 1.0f) - alignment;
		pointC = size * glm::vec2(1.0f, 1.0f) - alignment;
		pointD = size * glm::vec2(1.0f, 0.0f) - alignment;

		if (angle != 0.0f) { // with rotation
			oe::utils::rotatePoint(angle, pointA);
			oe::utils::rotatePoint(angle, pointB);
			oe::utils::rotatePoint(angle, pointC);
			oe::utils::rotatePoint(angle, pointD);
		}

		pointA += position;
		pointB += position;
		pointC += position;
		pointD += position;
	}

	void Renderer::submitVertex(const VertexData& vertex) {
		submitVertex(vertex, m_buffer_pos);
		m_buffer_pos++;
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color, glm::vec2 align, float angle)
	{
		submit(glm::vec3(position, 0.0f), size, sprite, color, align, angle);
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, glm::vec2 align, float angle)
	{
		submit(glm::vec3(position, 0.0f), size, color, align, angle);
	}

	void Renderer::submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, glm::vec2 align, float angle)
	{
		Sprite sprite(nullptr);
		submit(position, size, &sprite, color, align, angle);
	}

	void Renderer::submit(const glm::vec3& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color, glm::vec2 align, float angle)
	{
		if (!sprite) return;

		glm::vec2 pointA, pointB, pointC, pointD;
		gen_points(position, size, align, angle, pointA, pointB, pointC, pointD);

		submitVertex(VertexData(glm::vec3(pointA, position.z), sprite->position + sprite->size * glm::vec2(0.0f, 0.0f), color), m_buffer_pos++);
		submitVertex(VertexData(glm::vec3(pointB, position.z), sprite->position + sprite->size * glm::vec2(0.0f, 1.0f), color), m_buffer_pos++);
		submitVertex(VertexData(glm::vec3(pointC, position.z), sprite->position + sprite->size * glm::vec2(1.0f, 1.0f), color), m_buffer_pos++);
		submitVertex(VertexData(glm::vec3(pointD, position.z), sprite->position + sprite->size * glm::vec2(1.0f, 0.0f), color), m_buffer_pos++);
		m_vertex_count += 4;
	}

}
