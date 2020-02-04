#pragma once

#include "engine/graphics/renderer.h"

#include "engine/graphics/opengl/buffers/vertexArray.h"
#include "engine/graphics/opengl/buffers/vertexBuffer.h"
#include "engine/graphics/opengl/buffers/indexBuffer.h"



namespace oe::graphics {

	class GLRenderer : public Renderer {
	private:
		VertexArray* m_vao;
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;

		VertexData* m_mapped_buffer;
		size_t m_buffer_pos;

	public:
		GLRenderer(const RendererInfo& renderer_info);
		~GLRenderer();

		// Inherited via Renderer
		virtual void begin() override;
		virtual void end() override;

		virtual void submit(const VertexData& position) override;
		virtual void submit(const glm::vec2& position, const glm::vec2& size, const Sprite* sprite, const glm::vec4& color = glm::vec4(1.0f), glm::vec2 align = alignments::top_left, float angle = 0.0f, int quad_index = -1) override;
		
		virtual void clear() override;
		virtual void render(int quad_count = -1) const override;
	};

}
