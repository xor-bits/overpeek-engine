#include "renderer.h"

#include "buffers/vertexArray.h"
#include "buffers/buffer.h"
#include "../utility/math.h"
#include "../internal_libs.h"

#include <GL/glew.h>



namespace oe {

	Renderer::Renderer(types arrayRenderType, types indexRenderType, int _max_quad_count, void* staticVBOBuffer_data) {
		buffer_pos = 0;
		mapped_buffer = nullptr;

		quad_count = 0;
		max_quad_count = _max_quad_count;
		int max_vertices = max_quad_count * 4;
		int max_indices = max_quad_count * 6; // 0 1 2, 0 2 3

		// Vertex buffer
		float *vb;
		if (staticVBOBuffer_data == nullptr)
			vb = nullptr;
		else
			vb = (float*)staticVBOBuffer_data;

		// Index buffer
		unsigned short *ib = new unsigned short[max_indices];
		int index_counter = 0;
		for (int i = 0; i < max_quad_count; i++) {
			ib[i * 6 + 0] = index_counter + 0;
			ib[i * 6 + 1] = index_counter + 1;
			ib[i * 6 + 2] = index_counter + 2;
			ib[i * 6 + 3] = index_counter + 0;
			ib[i * 6 + 4] = index_counter + 2;
			ib[i * 6 + 5] = index_counter + 3;
			index_counter += 4;
		}

		// Shader buffers and attributes
		vao = new VertexArray();
		ibo = new IndexBuffer(ib, max_indices, indexRenderType);
		vbo = new VertexBuffer(vb, (size_t)max_vertices * (size_t)VertexData::component_count, VertexData::component_count, arrayRenderType);
		VertexData::configVBO(vbo);
	}

	Renderer::~Renderer() {
		delete vao;
		delete vbo;
		delete ibo;
	}

	void Renderer::begin() {
		vao->bind();
		vbo->bind();
		mapped_buffer = (VertexData*)vbo->mapBuffer();
	}

	void Renderer::end() {
		vao->bind();
		vbo->bind();
		vbo->unmapBuffer();
	}

	void Renderer::submitVertexData(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color) {
		submitVertexData(glm::vec3(position, 0.0f), size, texture, color);
	}

	void Renderer::submitVertexData(glm::vec3 position, glm::vec2 size, int texture, glm::vec4 color) {
		mapped_buffer[buffer_pos] = VertexData(position, size, texture, color);
		buffer_pos++;
	}

	void Renderer::submit(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color, alignments alignment, float angle) {
		submit(position, size, texture, color, alignment, angle, buffer_pos);
		buffer_pos += 4;
	}

	void Renderer::submit(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color, alignments alignment, float angle, int quad_index) {
		int initial_position = buffer_pos;
		buffer_pos = quad_index;

		glm::vec2 pointA = (size * glm::vec2(0.0f, 0.0f));
		glm::vec2 pointB = (size * glm::vec2(0.0f, 1.0f));
		glm::vec2 pointC = (size * glm::vec2(1.0f, 1.0f));
		glm::vec2 pointD = (size * glm::vec2(1.0f, 0.0f));

		pointA -= getAlignmentOffset(size, alignment);
		pointB -= getAlignmentOffset(size, alignment);
		pointC -= getAlignmentOffset(size, alignment);
		pointD -= getAlignmentOffset(size, alignment);

		if (angle != 0.0f) {

			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointA);
			rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointB);
			rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointC);
			rotatePoint(glm::vec2(0.0f, 0.0f), angle, pointD);
		}

		pointA += position;
		pointB += position;
		pointC += position;
		pointD += position;

		submitVertexData(pointA, glm::vec2(0.0f, 0.0f), texture, color);
		submitVertexData(pointB, glm::vec2(0.0f, 1.0f), texture, color);
		submitVertexData(pointC, glm::vec2(1.0f, 1.0f), texture, color);
		submitVertexData(pointD, glm::vec2(1.0f, 0.0f), texture, color);

		buffer_pos = initial_position;
	}

	void Renderer::clear() {
		buffer_pos = 0;
		quad_count = 0;
	}

	void Renderer::draw(int quad_count) const {
		if (quad_count == -1) quad_count = max_quad_count;

		//Binding
		vao->bind();
		vbo->bind();
		ibo->bind();

		if (quad_count > 0) {
			//glDrawArrays(GL_TRIANGLES, 0, 4);
			glDrawElements(GL_TRIANGLES, quad_count * 6, GL_UNSIGNED_SHORT, 0);
		}
	}

}