#pragma once

#include "engine/graphics/interface/poly_renderer.hpp"
#include "engine/utility/formatted_error.hpp"
#include "buffers/vertexArray.hpp"
#include "buffers/vertexBuffer.hpp"
#include "buffers/indexBuffer.hpp"
#include "gl.hpp"

#include <entt/signal/delegate.hpp>



namespace oe::graphics
{
	template<typename vertex_type>
	class GLBasicPolygonRenderer : public IBasicPolygonRenderer<vertex_type>
	{
	protected:
		VertexArray m_vao;
		VertexBuffer m_vbo;
		IndexBuffer m_ibo;

		entt::delegate<void(int32_t, int32_t)> m_render_call;

		using interface = IBasicPolygonRenderer<vertex_type>;

	protected:
		inline bool map_indices() noexcept override
		{
			if(interface::m_renderer_info.index_storage_type != oe::types::dynamic_type)
				return false;

			int32_t* data = reinterpret_cast<int32_t*>(m_ibo.mapBuffer());
			if(data == nullptr)
				return false;

			interface::m_indices = { data, data + m_ibo.getSize() };
			return true;
		}

		inline bool map_vertices() noexcept override
		{
			if(interface::m_renderer_info.vertex_storage_type != oe::types::dynamic_type)
				return false;

			vertex_type* data = reinterpret_cast<vertex_type*>(m_vbo.mapBuffer());
			if(data == nullptr)
				return false;

			interface::m_vertices = { data, data + m_vbo.getSize() };
			return true;
		}

	public:
		GLBasicPolygonRenderer(const PolygonRendererInfo& renderer_info)
			: interface(renderer_info)
			, m_vao()
			, m_vbo(interface::m_renderer_info.static_vertex_data, interface::m_renderer_info.vertex_count * sizeof(vertex_type),  vertex_type::component_count, interface::m_renderer_info.vertex_storage_type)
			, m_ibo(interface::m_renderer_info.static_index_data,  interface::m_renderer_info.index_count * sizeof(int32_t), interface::m_renderer_info.index_storage_type)
		{
			vertex_type::config([&](uint32_t index, int32_t components, int32_t startBytes) { m_vbo.attrib(index, components, startBytes); });
			primitive(oe::primitive_types::triangles);
		}

		~GLBasicPolygonRenderer() override
		{}
		
		void render(int32_t first, int32_t count) override
		{
			oe::Engine::draw_calls++;

			m_ibo.unmapBuffer();
			m_vbo.unmapBuffer();

			m_render_call(first, count);
		}

		template<oe::primitive_types primitives>
		constexpr void primitive()
		{
			m_render_call.connect(+[](const void* /* fn */, int32_t first, int32_t count)
			{
				gl::drawElements<primitives, uint32_t>(first, count);
			});
		}

		void primitive(oe::primitive_types primitives) override
		{
			switch (primitives)
			{
			case oe::primitive_types::points:
				primitive<oe::primitive_types::points>();
				break;
			case oe::primitive_types::lines:
				primitive<oe::primitive_types::lines>();
				break;
			case oe::primitive_types::triangles:
				primitive<oe::primitive_types::triangles>();
				break;
			
			default:
				throw oe::utils::formatted_error("Invalid primitive type {}", static_cast<size_t>(primitives));
				break;
			}
		}
	};
}