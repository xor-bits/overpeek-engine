#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/vertexData.hpp"
#include "engine/graphics/sprite.hpp"
#include "engine/graphics/interface/instance.hpp"



namespace oe::graphics {

	/*Batched quad renderer*/
	class Renderer {
	public:
		RendererInfo m_renderer_info;

	protected:
		size_t m_vertex_count;
		size_t m_buffer_pos;

	public:
		Renderer(const RendererInfo& renderer_info);
		~Renderer();

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void submitVertex(const VertexData& vertex, int index) = 0;

		// nullptr sprite will submit nothing
		void submit(const glm::vec3& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f);
		void submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f);
		void submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f);
		void submit(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f);

		virtual void clear() = 0;
		virtual void render(size_t override_vertex_count) const = 0;
		void render() { render(m_vertex_count);  };

		size_t& vertexCount() { return m_vertex_count; }
		size_t maxQuadCount() const { return m_renderer_info.max_quad_count; }
	};

}
