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
		m_buffer_pos = 0;
		m_mapped_buffer = nullptr;

		int max_vertices = m_max_quad_count * 4;
		int max_indices = m_max_quad_count * 6; // 0 1 2, 0 2 3

		// Vertex buffer
		float *vb;
		if (renderer_info.staticVBOBuffer_data == nullptr)
			vb = nullptr;
		else
			vb = (float*)renderer_info.staticVBOBuffer_data;

		// Index buffer
		unsigned short *ib = new unsigned short[max_indices];
		int index_counter = 0;
		for (int i = 0; i < m_max_quad_count; i++) {
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
		m_vbo = new VertexBuffer(vb, (size_t)max_vertices * (size_t)VertexData::component_count * sizeof(float), VertexData::component_count, renderer_info.arrayRenderType);
		m_vbo->config();

		delete[] ib;
	}

	GLRenderer::~GLRenderer() {
		delete m_vao;
		delete m_vbo;
		delete m_ibo;
	}

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

	void GLRenderer::submit(const VertexData& position)
	{
		if (!m_mapped_buffer) oe_error_terminate("Buffer not mapped!");

		m_mapped_buffer[m_buffer_pos] = position;
		m_buffer_pos++;
	}

	void GLRenderer::submit(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, glm::vec2 align, float angle, int quad_index)
	{
		Sprite sprite(nullptr);
		submit(position, size, &sprite, color, align, angle, quad_index);
	}

	void GLRenderer::submit(const glm::vec3& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color, glm::vec2 align, float angle, int quad_index)
	{
		if (!sprite) return;

		glm::vec2 pointA, pointB, pointC, pointD;
		gen_points(position, size, align, angle, pointA, pointB, pointC, pointD);

		int before;
		if (quad_index != -1) {
			before = m_buffer_pos;
			m_buffer_pos = quad_index;
		}

		submit(VertexData(glm::vec3(pointA, position.z), sprite->position + sprite->size * glm::vec2(0.0f, 0.0f), color));
		submit(VertexData(glm::vec3(pointB, position.z), sprite->position + sprite->size * glm::vec2(0.0f, 1.0f), color));
		submit(VertexData(glm::vec3(pointC, position.z), sprite->position + sprite->size * glm::vec2(1.0f, 1.0f), color));
		submit(VertexData(glm::vec3(pointD, position.z), sprite->position + sprite->size * glm::vec2(1.0f, 0.0f), color));

		if (quad_index != -1) {
			before = m_buffer_pos;
		}
		else {
			m_quad_count++;
		}
	}

	void GLRenderer::clear()
	{
		m_buffer_pos = 0;
		m_quad_count = 0;
	}

	void GLRenderer::render(int quad_count) const
	{
		if (quad_count == -1) quad_count = m_quad_count;

		//Binding
		m_vao->bind();

		if (quad_count > 0) {
			glDrawElements(GL_TRIANGLES, quad_count * 6, GL_UNSIGNED_SHORT, 0);
		}
	}

}
