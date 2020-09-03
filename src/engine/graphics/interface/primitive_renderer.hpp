#pragma once

#include "engine/internal_libs.hpp"

#include "engine/enum.hpp"
#include "engine/graphics/interface/index_buffer_gen.hpp"
#include "engine/graphics/vertexData.hpp"
#include "engine/utility/extra.hpp"
#include "engine/engine.hpp"



namespace oe::graphics {

	template<primitive_types type, typename buffer_gen, typename vertex_type>
	class IBasicPrimitiveRenderer {
	public:
		RendererInfo m_renderer_info;

	protected:
		size_t m_vertex_count;
		size_t m_buffer_pos;
		size_t m_index_per_primitive;
		size_t m_vertex_per_primitive;

	public:
		IBasicPrimitiveRenderer(const RendererInfo& renderer_info)
			: m_renderer_info(renderer_info)
			, m_vertex_count(0)
			, m_buffer_pos(0)
		{}

		virtual ~IBasicPrimitiveRenderer()
		{}

		virtual vertex_type* modifyVertex(size_t vertex_count, size_t first_index = 0) = 0;
		virtual void submitVertex(const vertex_type* first_vertex, size_t vertex_count, size_t first_index = 0) = 0;

		void submitVertex(const vertex_type& vertex, size_t index = 0)
		{
			return submitVertex(&vertex, 1, index);
		}
		
		template<typename arr_type>
		void submitVertex(const arr_type& arr, size_t first_index = 0, typename std::enable_if<oe::is_container<arr_type>::value >::type* = 0)
		{
			return submitVertex(arr.data(), arr.size(), first_index);
		}
		
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
			Engine::draw_calls++;
			render(primitive_count);
		}

	};

	// typedef IBasicPrimitiveRenderer<primitive_types::quads, BasicBufferGen<primitive_types::quads>, VertexData> IPrimitiveRenderer;
	using IPrimitiveRenderer = IBasicPrimitiveRenderer<primitive_types::quads, BasicBufferGen<primitive_types::quads>, VertexData>;

};
