#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/renderer.hpp"



namespace oe::graphics {

	class VKWindow; class VKBuffer;

	template<primitive_types type = primitive_types::quads, typename buffer_gen = basic_buffer_gen>
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
		void begin() override;
		void end() override;
		
		void submitVertex(const VertexData& vertex, int index) override;
		
		void clear() override;
		void render(size_t override_vertex_count) const override;
	
	};

}
#endif