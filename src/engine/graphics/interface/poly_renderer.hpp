#pragma once

#include <gsl/span>

#include "engine/enum.hpp"
#include "engine/engine.hpp"



namespace oe::graphics
{
	template<typename vertex_type>
	class IBasicPolygonRenderer
	{
	protected:
		PolygonRendererInfo m_renderer_info;
		gsl::span<int32_t> m_indices;
		gsl::span<vertex_type> m_vertices;
		
		virtual bool map_indices() noexcept = 0;
		virtual bool map_vertices() noexcept = 0;

	public:
		IBasicPolygonRenderer(const PolygonRendererInfo& renderer_info)
			: m_renderer_info(renderer_info)
		{}

		virtual ~IBasicPolygonRenderer()
		{}
		
		[[nodiscard]] gsl::span<int32_t>* getIndices() noexcept
		{
			if(map_indices())
				return &m_indices;

			return nullptr;
		}

		[[nodiscard]] gsl::span<vertex_type>* getVertices() noexcept
		{
			if(map_vertices())
				return &m_vertices;

			return nullptr;
		}
		
		// render all indices
		// oe::primitive_types::quads not allowed
		virtual void render(int32_t first = 0, int32_t count = std::numeric_limits<int32_t>::max()) = 0;

		// set the render primitive type
		// defaults to triangles
		virtual void primitive(oe::primitive_types primitives = oe::primitive_types::triangles) = 0;
	};
}