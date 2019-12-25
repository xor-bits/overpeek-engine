#pragma once

#include "internal_libs.h"
#include "graphics/buffers/vertexArray.h"
#include "graphics/buffers/vertexBuffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/vertexData.h"



namespace oe::graphics {

	enum class alignment
	{
		top_left, center_left, bottom_left, top_center, center_center, bottom_center, top_right, center_right, bottom_right
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
		Renderer(types arrayRenderType, types indexRenderType, int max_quad_count, void* staticVBOBuffer_data = nullptr);
		~Renderer();

		void begin();
		void end();
		void submitVertexData(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color);
		void submitVertexData(const glm::vec3& position, const glm::vec2& size, int texture, const glm::vec4& color);
		void submit(const glm::vec2& position, const glm::vec2& size, int texture = 0, const glm::vec4& color = glm::vec4(1.0f));
		void submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align);
		void submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align, float angle);
		void submit(const glm::vec2& position, const glm::vec2& size, int texture, const glm::vec4& color, alignment align, float angle, int quad_index);

		void clear();
		void render(int quad_count = -1) const;

		size_t getQuadCount();
		void incrementQuadCount(int n = 1);
	};

}
