#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/interface/renderer.hpp"



namespace oe::graphics {

	class VKWindow; class VKBuffer;

	template<primitive_types type = primitive_types::quads, class buffer_gen = basic_buffer_gen>
	class VKRenderer : public Renderer<type, buffer_gen> {
	private:
		const VKWindow* m_window;
		VKBuffer* m_vbo;
		VKBuffer* m_ibo;

		VertexData* m_mapped_buffer;

	public:
		VKRenderer(const VKWindow* window, const RendererInfo& renderer_info);
		~VKRenderer();

		// Inherited via Renderer
		virtual void begin() override;
		virtual void end() override;
		
		virtual void submitVertex(const VertexData& vertex, int index) override;
		
		virtual void clear() override;
		virtual void render(size_t override_vertex_count) const override;
	
	};

}
#endif