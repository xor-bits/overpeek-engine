#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/sprite.hpp"

#include "engine/enum.hpp"
#include "engine/graphics/interface/index_buffer_gen.hpp"
#include "engine/graphics/vertexData.hpp"
#include "engine/utility/extra.hpp"



namespace oe::graphics {

	template<primitive_types type, typename buffer_gen, typename vertex_type>
	class BasicPrimitiveRenderer {
	public:
		RendererInfo m_renderer_info;

	protected:
		size_t m_vertex_count;
		size_t m_buffer_pos;
		size_t m_index_per_primitive;
		size_t m_vertex_per_primitive;

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

	public:
		BasicPrimitiveRenderer(const RendererInfo& renderer_info)
			: m_renderer_info(renderer_info)
			, m_vertex_count(0)
			, m_buffer_pos(0)
		{}

		~BasicPrimitiveRenderer()
		{}

		virtual void submitVertex(const vertex_type& vertex, size_t index) = 0;
		virtual void submitVertex(const std::vector<vertex_type>& vertices, size_t first_index) = 0;
		virtual void submitVertex(const vertex_type* first_vertex, size_t vertex_count, size_t first_index) = 0;
		
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void render(size_t override_primitive_count) const = 0;

	public:
		void submitVertex(const vertex_type& vertex)
		{
			submitVertex(vertex, m_buffer_pos);
			m_buffer_pos++;
		}

		size_t& bufferPos()
		{
			return m_buffer_pos;
		}

		size_t& vertexCount() 
		{ 
			return m_vertex_count; 
		}

		size_t maxQuadCount() const 
		{ 
			return m_renderer_info.max_primitive_count; 
		}

		void clear()
		{
			m_buffer_pos = 0;
			m_vertex_count = 0;
		}

		void render() 
		{
			size_t primitive_count = m_vertex_count / m_vertex_per_primitive;
			render(primitive_count);
		}

	};

	typedef BasicPrimitiveRenderer<primitive_types::quads, BasicBufferGen<primitive_types::quads>, VertexData> PrimitiveRenderer;

};
