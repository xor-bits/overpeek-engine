#pragma once

#include "engine/graphics/interface/primitive_renderer.hpp"

#include "engine/graphics/opengl/buffers/vertexArray.hpp"
#include "engine/graphics/opengl/buffers/vertexBuffer.hpp"
#include "engine/graphics/opengl/buffers/indexBuffer.hpp"

// #include "engine/engine.hpp"



namespace oe::graphics
{
	extern void native_glDrawElementsPoints   (const int32_t override_index_offset, const int32_t override_index_count);
	extern void native_glDrawElementsLines    (const int32_t override_index_offset, const int32_t override_index_count);
	extern void native_glDrawElementsTriangles(const int32_t override_index_offset, const int32_t override_index_count);



	template<oe::primitive_types type, typename buffer_gen, typename vertex_type>
	class GLBasicPrimitiveRenderer : public IBasicPrimitiveRenderer<type, buffer_gen, vertex_type> {
	private:
		VertexArray* m_vao;
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;

		vertex_type* m_mapped_buffer;

		void(*native_glDrawElementsPrimitive)(int32_t,int32_t);
		using Interface = IBasicPrimitiveRenderer<type, buffer_gen, vertex_type>;

	public:
		GLBasicPrimitiveRenderer(const RendererInfo& renderer_info)
			: Interface(renderer_info)
		{
			oe_debug_call("gl_basic_primitive_renderer");
			m_mapped_buffer = nullptr;
			buffer_gen generator(renderer_info.max_primitive_count);
			this->m_index_per_primitive = generator.index_per_primitive();
			this->m_vertex_per_primitive = generator.vertex_per_primitive();

			// Buffers
			auto vertex_buffer_data = generator.template optional_vertex_gen<vertex_type>(renderer_info.staticVBOBuffer_data);
			auto index_buffer_data = generator.gen();

			// IShader buffers and attributes
			m_vao = new VertexArray();
			m_ibo = new IndexBuffer(index_buffer_data.ptr(), index_buffer_data.size(), renderer_info.indexRenderType);
			m_vbo = new VertexBuffer(vertex_buffer_data.ptr(), vertex_buffer_data.size(), vertex_type::component_count, renderer_info.arrayRenderType);
			vertex_type::config([&](uint32_t index, int32_t components, int32_t startBytes) { m_vbo->attrib(index, components, startBytes); });

			oe::primitive_types ptype = generator.render_primitive();
			switch (ptype)
			{
			case oe::primitive_types::points:
				native_glDrawElementsPrimitive = &native_glDrawElementsPoints;
				break;
			case oe::primitive_types::lines:
				native_glDrawElementsPrimitive = &native_glDrawElementsLines;
				break;
			case oe::primitive_types::triangles:
				native_glDrawElementsPrimitive = &native_glDrawElementsTriangles;
				break;
			default:
				oe_error_terminate("Non primitive oe::primitive_types asked: {}", static_cast<int>(ptype));
				native_glDrawElementsPrimitive = &native_glDrawElementsTriangles;
				break;
			}
		}

		~GLBasicPrimitiveRenderer()
		{
			delete m_vao;
			delete m_vbo;
			delete m_ibo;
		}

		// Inherited via Renderer
		virtual vertex_type* modifyVertex(int32_t vertex_count, int32_t first_index) override
		{
			if (!m_mapped_buffer)
				oe_error_terminate("Buffer not mapped!");

			if (first_index + vertex_count > m_vbo->getSize() || vertex_count < 0 || first_index < 0)
				return nullptr;

			return m_mapped_buffer + first_index;
		}

		virtual void submitVertex(const vertex_type* first_vertex, int32_t vertex_count, int32_t first_index) override
		{
			if (!m_mapped_buffer)
				oe_error_terminate("Buffer not mapped!");

			first_index = std::abs(first_index);
			vertex_count = std::clamp(vertex_count, 0, m_vbo->getSize() * static_cast<int32_t>(sizeof(vertex_type)) - first_index);

			std::memcpy(m_mapped_buffer + first_index, first_vertex, vertex_count * sizeof(vertex_type));
			Interface::m_vertex_count = std::max(Interface::m_vertex_count, first_index + vertex_count);
		}

		virtual void begin() override
		{
			m_vao->bind();
			m_vbo->bind();
			m_mapped_buffer = (vertex_type*)m_vbo->mapBuffer();
		}

		virtual void end() override
		{
			m_vao->bind();
			m_vbo->bind();
			if (m_mapped_buffer) m_vbo->unmapBuffer();
		}

		virtual void _render(int32_t override_primitive_offset, int32_t override_primitive_count) const override
		{
			if(override_primitive_count == std::numeric_limits<int32_t>::max())
				override_primitive_count = m_vertex_count / this->m_vertex_per_primitive;

			if (override_primitive_count > 0) {
				m_vao->bind();
				native_glDrawElementsPrimitive(override_primitive_offset * this->m_index_per_primitive, override_primitive_count * this->m_index_per_primitive);
			}
		}

		const VertexBuffer* getVBO() const
		{
			return m_vbo;
		}

		const IndexBuffer* getIBO() const
		{
			return m_ibo;
		}
	};

	using GLPrimitiveRenderer = GLBasicPrimitiveRenderer<oe::primitive_types::quads, BasicBufferGen<oe::primitive_types::quads>, VertexData>;

}
