#pragma once

#include "engine/graphics/interface/renderer.hpp"

#include "engine/graphics/opengl/buffers/vertexArray.hpp"
#include "engine/graphics/opengl/buffers/vertexBuffer.hpp"
#include "engine/graphics/opengl/buffers/indexBuffer.hpp"



namespace oe::graphics {

	class GLRenderer : public Renderer {
	private:
		VertexArray* m_vao;
		VertexBuffer* m_vbo;
		IndexBuffer* m_ibo;

		VertexData* m_mapped_buffer;

	public:
		GLRenderer(const RendererInfo& renderer_info);
		~GLRenderer();

		// Inherited via Renderer
		virtual void begin() override;
		virtual void end() override;

		virtual void submitVertex(const VertexData& vertex, int index) override;
		
		virtual void clear() override;
		virtual void render(size_t override_vertex_count) const override;
	};

}
