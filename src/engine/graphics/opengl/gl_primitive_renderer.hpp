#pragma once

#include "engine/graphics/interface/primitive_renderer.hpp"

#include "engine/graphics/opengl/buffers/vertexArray.hpp"
#include "engine/graphics/opengl/buffers/vertexBuffer.hpp"
#include "engine/graphics/opengl/buffers/indexBuffer.hpp"

#include "engine/engine.hpp"



namespace oe::graphics {

	void native_glDrawElementsPoints(size_t override_index_count);
	void native_glDrawElementsLines(size_t override_index_count);
	void native_glDrawElementsTriangles(size_t override_index_count);



	template<oe::primitive_types type, typename buffer_gen, typename vertex_type>
	class GLBasicPrimitiveRenderer : public BasicPrimitiveRenderer<type, buffer_gen, vertex_type> {
	private:
		VertexArray* m_vao;
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;

		vertex_type* m_mapped_buffer;

		std::function<void(size_t)> native_glDrawElementsPrimitive;

	public:
		GLBasicPrimitiveRenderer(const RendererInfo& renderer_info)
			: BasicPrimitiveRenderer<type, buffer_gen, vertex_type>(renderer_info)
		{
			oe_debug_call("gl_basic_primitive_renderer");
			m_mapped_buffer = nullptr;
			buffer_gen generator(renderer_info.max_primitive_count);
			this->m_index_per_primitive = generator.index_per_primitive();
			this->m_vertex_per_primitive = generator.vertex_per_primitive();

			// Buffers
			auto vertex_buffer_data = generator.template optional_vertex_gen<vertex_type>(renderer_info.staticVBOBuffer_data);
			auto index_buffer_data = generator.gen();

			// Shader buffers and attributes
			m_vao = new VertexArray();
			m_ibo = new IndexBuffer(index_buffer_data.ptr(), index_buffer_data.size(), renderer_info.indexRenderType);
			m_vbo = new VertexBuffer(vertex_buffer_data.ptr(), vertex_buffer_data.size(), vertex_type::component_count, renderer_info.arrayRenderType);
			std::function<void(int, int, size_t)> fn = std::bind(&VertexBuffer::attrib, m_vbo, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			vertex_type::config(fn);

			switch (generator.render_primitive())
			{
			case oe::primitive_types::points:
				native_glDrawElementsPrimitive = native_glDrawElementsPoints;
			case oe::primitive_types::lines:
				native_glDrawElementsPrimitive = native_glDrawElementsLines;
			case oe::primitive_types::triangles:
				native_glDrawElementsPrimitive = native_glDrawElementsTriangles;
			}
		}

		~GLBasicPrimitiveRenderer() {
			delete m_vao;
			delete m_vbo;
			delete m_ibo;
		}

		// Inherited via Renderer
		virtual void submitVertex(const vertex_type& vertex, size_t index) override {
			if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
			m_mapped_buffer[index] = vertex;
			m_vertex_count = std::max(m_vertex_count, index + 1);
		}

		virtual void submitVertex(const std::vector<vertex_type>& vertices, size_t first_index) override {
			if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
			std::memcpy(m_mapped_buffer + first_index, vertices.data(), vertices.size() * sizeof(vertex_type));
			m_vertex_count = std::max(m_vertex_count, first_index + vertices.size());
		}
		
		virtual void submitVertex(const vertex_type* first_vertex, size_t vertex_count, size_t first_index) override {
			if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
			std::memcpy(m_mapped_buffer + first_index, first_vertex, vertex_count * sizeof(vertex_type));
			m_vertex_count = std::max(m_vertex_count, first_index + vertex_count);
		}

		virtual void begin() override {
			m_vao->bind();
			m_vbo->bind();
			m_mapped_buffer = (vertex_type*)m_vbo->mapBuffer();
			if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
		}

		virtual void end() override {
			m_vao->bind();
			m_vbo->bind();
			m_vbo->unmapBuffer();
		}

		virtual void render(size_t override_primitive_count) const override {
			if (override_primitive_count > 0) {
				m_vao->bind();
				native_glDrawElementsPrimitive(override_primitive_count * this->m_index_per_primitive);
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

	typedef GLBasicPrimitiveRenderer<oe::primitive_types::quads, BasicBufferGen<oe::primitive_types::quads>, VertexData> GLPrimitiveRenderer;

}
