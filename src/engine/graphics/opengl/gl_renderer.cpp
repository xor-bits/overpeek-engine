#include "gl_renderer.hpp"

#include "engine/engine.hpp"
#include "buffers/vertexArray.hpp"
#include "buffers/buffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/vec_swizzle.hpp>



namespace oe::graphics {

	GLRenderer::GLRenderer(const RendererInfo& renderer_info)
		: Renderer(renderer_info) 
	{
		m_mapped_buffer = nullptr;

		int max_vertices = m_renderer_info.max_quad_count * 4;
		int max_indices = m_renderer_info.max_quad_count * 6; // 0 1 2, 0 2 3

		// Vertex buffer
		float *vb;
		size_t size = (size_t)max_vertices * (size_t)VertexData::component_count * sizeof(float);
		if (renderer_info.staticVBOBuffer_data == nullptr) {
			vb = nullptr;
		}
		else {
			vb = new float[size];
			std::memcpy(vb, renderer_info.staticVBOBuffer_data, size);
		}

		// Index buffer
		unsigned short *ib = new unsigned short[max_indices];
		int index_counter = 0;
		for (int i = 0; i < m_renderer_info.max_quad_count; i++) {
			ib[i * 6 + 0] = index_counter + 0;
			ib[i * 6 + 1] = index_counter + 1;
			ib[i * 6 + 2] = index_counter + 2;
			ib[i * 6 + 3] = index_counter + 0;
			ib[i * 6 + 4] = index_counter + 2;
			ib[i * 6 + 5] = index_counter + 3;
			index_counter += 4;
		}

		// Shader buffers and attributes
		m_vao = new VertexArray();
		m_ibo = new IndexBuffer(ib, max_indices * sizeof(unsigned short), renderer_info.indexRenderType);
		m_vbo = new VertexBuffer(vb, size, VertexData::component_count, renderer_info.arrayRenderType);
		m_vbo->config();

		delete[] ib;
	}

	GLRenderer::~GLRenderer() {
		delete m_vao;
		delete m_vbo;
		delete m_ibo;
	}

	void GLRenderer::begin()
	{
		m_vao->bind();
		m_vbo->bind();
		m_mapped_buffer = (VertexData*)m_vbo->mapBuffer();
		if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
	}

	void GLRenderer::end()
	{
		m_vao->bind();
		m_vbo->bind();
		m_vbo->unmapBuffer();
	}

	void GLRenderer::submitVertex(const VertexData& position, int index)
	{
		if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");
		m_mapped_buffer[index] = position;
	}

	void GLRenderer::clear()
	{
		m_buffer_pos = 0;
		m_vertex_count = 0;
	}

	void GLRenderer::render(size_t override_vertex_count) const
	{
		if (override_vertex_count > 0) {
			m_vao->bind();
			glDrawElements(GL_TRIANGLES, override_vertex_count / 4 * 6, GL_UNSIGNED_SHORT, 0);
		}
	}

}
