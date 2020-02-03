#pragma once

#include "engine/internal_libs.h"
#include "engine/graphics/buffers/vertexArray.h"
#include "engine/graphics/buffers/vertexBuffer.h"
#include "engine/graphics/buffers/indexBuffer.h"
#include "engine/graphics/vertexData.h"
#include "engine/graphics/sprite.h"



namespace oe::graphics {

	enum alignment
	{
		top_left, center_left, bottom_left, top_center, center_center, bottom_center, top_right, center_right, bottom_right, last
	};

	static glm::vec2 alignmentOffset(const glm::vec2& size, alignment align) {
		switch (align)
		{
		case alignment::top_left:
			return size * glm::vec2(0.0f, 0.0f);
		case alignment::center_left:
			return size * glm::vec2(0.0f, 0.5f);
		case alignment::bottom_left:
			return size * glm::vec2(0.0f, 1.0f);
		case alignment::top_center:
			return size * glm::vec2(0.5f, 0.0f);
		case alignment::center_center:
			return size * glm::vec2(0.5f, 0.5f);
		case alignment::bottom_center:
			return size * glm::vec2(0.5f, 1.0f);
		case alignment::top_right:
			return size * glm::vec2(1.0f, 0.0f);
		case alignment::center_right:
			return size * glm::vec2(1.0f, 0.5f);
		case alignment::bottom_right:
			return size * glm::vec2(1.0f, 1.0f);
		default:
			return size * glm::vec2(0.0f, 0.0f);
		}
	}

	enum types {
		staticrender, dynamicrender
	};

	/*Batched quad renderer*/
	class Renderer {
	private:
		VertexArray * m_vao;
		VertexBuffer * m_vbo;
		IndexBuffer * m_ibo;

		int m_max_quad_count;
		int m_quad_count;
		VertexData * m_mapped_buffer;
		int m_buffer_pos;

	public:
		Renderer(int max_quad_count = 10000);
		Renderer(types arrayRenderType, types indexRenderType, int max_quad_count, void* staticVBOBuffer_data = nullptr);
		~Renderer();

		void begin();
		void end();
		void submitVertexData(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color);
		void submitVertexData(const glm::vec3& position, const glm::vec2& size, int texture, const glm::vec4& color);

		void submit(const glm::vec2& position, const glm::vec2& size, int texture = 0, const glm::vec4& color = glm::vec4(1.0f), alignment align = alignment::top_left, float angle = 0.0f, int quad_index = -1);

		// nullptr sprite will submit nothing
		void submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), alignment align = alignment::top_left, float angle = 0.0f, int quad_index = -1);
		void submitFast(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f));

		void clear();
		void render(int quad_count = -1) const;

		size_t getQuadCount();
		void incrementQuadCount(int n = 1);
	};

}
