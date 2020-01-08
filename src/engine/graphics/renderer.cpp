#include "renderer.h"

#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "../utility/math.h"
#include "../internal_libs.h"
#include "engine/graphics/gl.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace oe::graphics {

	Renderer::Renderer(types arrayRenderType, types indexRenderType, int _max_quad_count, void* staticVBOBuffer_data) {
		m_buffer_pos = 0;
		m_mapped_buffer = nullptr;

		m_quad_count = 0;
		m_max_quad_count = _max_quad_count;
		int max_vertices = m_max_quad_count * 4;
		int max_indices = m_max_quad_count * 6; // 0 1 2, 0 2 3

		// Vertex buffer
		float *vb;
		if (staticVBOBuffer_data == nullptr)
			vb = nullptr;
		else
			vb = (float*)staticVBOBuffer_data;

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
		m_ibo = new IndexBuffer(ib, max_indices * sizeof(unsigned short), indexRenderType);
		m_vbo = new VertexBuffer(vb, (size_t)max_vertices * (size_t)VertexData::component_count * sizeof(float), VertexData::component_count, arrayRenderType);
		VertexData::configVBO(m_vbo);
	}

	Renderer::~Renderer() {
		delete m_vao;
		delete m_vbo;
		delete m_ibo;
	}

	void Renderer::begin() {
		m_vao->bind();
		m_vbo->bind();
		m_mapped_buffer = (VertexData*)m_vbo->mapBuffer();
	}

	void Renderer::end() {
		m_vao->bind();
		m_vbo->bind();
		m_vbo->unmapBuffer();
	}

	void Renderer::submitVertexData(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color) {
		submitVertexData(glm::vec3(position, 0.0f), size, texture, color);
	}

	void Renderer::submitVertexData(const glm::vec3& position, const glm::vec2& size, int texture, const glm::vec4& color) {
		if (!m_mapped_buffer) { spdlog::error("Buffer not mapped!"); return; }
		m_mapped_buffer[m_buffer_pos].position = position;
		m_mapped_buffer[m_buffer_pos].size_or_uv = size;
		m_mapped_buffer[m_buffer_pos].texture = texture;
		m_mapped_buffer[m_buffer_pos].color = color;
		m_buffer_pos++;
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color) {
		submitVertexData(position + size * glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), texture, color);
		submitVertexData(position + size * glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f), texture, color);
		submitVertexData(position + size * glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), texture, color);
		submitVertexData(position + size * glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f), texture, color);

		incrementQuadCount();
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align) {
		submit(position - alignmentOffset(size, align), size, texture, color);
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align, float angle) {
		submit(position, size, texture, color, align, angle, m_buffer_pos);
		m_buffer_pos += 4;
		m_quad_count++;
	}

	void Renderer::submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align, float angle, int quad_index) {
		int initial_position = m_buffer_pos;
		m_buffer_pos = quad_index;

		glm::vec2 pointA = (size * glm::vec2(0.0f, 0.0f));
		glm::vec2 pointB = (size * glm::vec2(0.0f, 1.0f));
		glm::vec2 pointC = (size * glm::vec2(1.0f, 1.0f));
		glm::vec2 pointD = (size * glm::vec2(1.0f, 0.0f));

		pointA -= alignmentOffset(size, align);
		pointB -= alignmentOffset(size, align);
		pointC -= alignmentOffset(size, align);
		pointD -= alignmentOffset(size, align);

		if (angle != 0.0f) {

			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			oe::utils::rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointA);
			oe::utils::rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointB);
			oe::utils::rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointC);
			oe::utils::rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointD);
		}

		pointA += position;
		pointB += position;
		pointC += position;
		pointD += position;

		submitVertexData(pointA, glm::vec2(0.0f, 0.0f), texture, color);
		submitVertexData(pointB, glm::vec2(0.0f, 1.0f), texture, color);
		submitVertexData(pointC, glm::vec2(1.0f, 1.0f), texture, color);
		submitVertexData(pointD, glm::vec2(1.0f, 0.0f), texture, color);

		m_buffer_pos = initial_position;
	}

	void Renderer::clear() {
		m_buffer_pos = 0;
		m_quad_count = 0;
	}

	void Renderer::render(int quad_count) const {
		if (quad_count == -1) quad_count = m_quad_count;

		//Binding
		m_vao->bind();

		if (quad_count > 0) {
			glDrawElements(GL_TRIANGLES, quad_count * 6, GL_UNSIGNED_SHORT, 0);
		}
	}

	size_t Renderer::getQuadCount() {
		return m_quad_count;
	}
	
	void Renderer::incrementQuadCount(int n) {
		m_quad_count += n;
	}

}
