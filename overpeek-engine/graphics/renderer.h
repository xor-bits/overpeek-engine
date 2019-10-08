#pragma once

#include "../internal_libs.h"

#include "buffers/vertexArray.h"
#include "buffers/vertexBuffer.h"
#include "buffers/indexBuffer.h"

#include "vertexData.h"



namespace oe {

	struct alignments
	{
		glm::vec2 a;
	};
	static alignments topLeft		{ glm::vec2(0.0f, 0.0f) };
	static alignments centerLeft	{ glm::vec2(0.0f, 0.5f) };
	static alignments bottomLeft	{ glm::vec2(0.0f, 1.0f) };
	static alignments topCenter		{ glm::vec2(0.5f, 0.0f) };
	static alignments centerCenter	{ glm::vec2(0.5f, 0.5f) };
	static alignments bottomCenter	{ glm::vec2(0.5f, 1.0f) };
	static alignments topRight		{ glm::vec2(1.0f, 0.0f) };
	static alignments centerRight	{ glm::vec2(1.0f, 0.5f) };
	static alignments bottomRight	{ glm::vec2(1.0f, 1.0f) };
	static glm::vec2 getAlignmentOffset(glm::vec2 size, alignments a) {
		return (size * a.a);
	}

	const enum types {
		staticDraw, dynamicDraw
	};

	/*Batched quad renderer*/
	class Renderer {
	private:
		VertexArray *vao;
		VertexBuffer *vbo;
		IndexBuffer *ibo;

		int max_quad_count;
		int quad_count;
		VertexData *mapped_buffer;
		int buffer_pos;

	public:
		Renderer(types arrayRenderType, types indexRenderType, int max_quad_count, void* staticVBOBuffer_data = nullptr);
		~Renderer();

		void begin();
		void end();
		void submitVertexData(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color);
		void submitVertexData(glm::vec3 position, glm::vec2 size, int texture, glm::vec4 color);
		void submit(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color, alignments alignment, float angle);
		void submit(glm::vec2 position, glm::vec2 size, int texture, glm::vec4 color, alignments alignment, float angle, int quad_index);

		void clear();
		void draw(int quad_count = -1) const;

	};

}