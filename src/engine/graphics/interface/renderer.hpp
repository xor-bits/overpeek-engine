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
		size_t m_quad_count;
		size_t m_max_quad_count;

	public:
		Renderer(const RendererInfo& renderer_info);
		~Renderer();

		virtual void begin() = 0;
		virtual void end() = 0;

		// nullptr sprite will submit nothing
		virtual void submit(const VertexData& position) = 0;
		virtual void submit(const glm::vec3& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f, int quad_index = -1) = 0;
		virtual void submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f, int quad_index = -1) = 0;
		void submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f, int quad_index = -1);
		void submit(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f, int quad_index = -1);

		virtual void clear() = 0;
		virtual void render(int quad_count = -1) const = 0;

		size_t& quadCount() { return m_quad_count; }
		size_t maxQuadCount() const { return m_max_quad_count; }
	};

}
