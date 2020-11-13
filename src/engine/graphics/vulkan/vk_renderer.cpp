#pragma once

#include "vk_renderer.hpp"
#ifdef BUILD_VULKAN

#include "engine/graphics/vulkan/vk_window.hpp"
#include "engine/graphics/vulkan/buffers/vk_command_pool.hpp"
#include "engine/graphics/vulkan/buffers/vk_buffer.hpp"



namespace oe::graphics {

	VKRenderer::VKRenderer(const VKWindow* window, const RendererInfo& renderer_info)
		: Renderer(renderer_info)
		, m_window(window)
	{
		m_mapped_buffer = nullptr;

		int max_vertices = m_renderer_info.max_quad_count * 4;
		int max_indices = m_renderer_info.max_quad_count * 6; // 0 1 2, 0 2 3

		size_t vbo_size = (size_t)max_vertices * (size_t)VertexData::component_count * sizeof(float);
		size_t ibo_size = (size_t)max_indices * sizeof(unsigned short);

		// Vertex buffer
		m_vbo = new VKBuffer(vk::BufferUsageFlagBits::eVertexBuffer, vbo_size, m_window->m_logical_device, m_window);
		if (renderer_info.staticVBOBuffer_data != nullptr) {
			std::memcpy(m_vbo->map(), renderer_info.staticVBOBuffer_data, vbo_size);
			m_vbo->unmap();
		}

		// Index buffer
		m_ibo = new VKBuffer(vk::BufferUsageFlagBits::eIndexBuffer, ibo_size, m_window->m_logical_device, m_window);
		unsigned short* ib = new unsigned short[max_indices];
		int index_counter = 0;
		for (int i = 0; i < m_renderer_info.max_quad_count; i++) {
			ib[i * 6 + 0] = index_counter + 0;
			ib[i * 6 + 1] = index_counter + 1;
			ib[i * 6 + 2] = index_counter + 2;
			ib[i * 6 + 3] = index_counter + 0;
			ib[i * 6 + 4] = index_counter + 2;
			ib[i * 6 + 5] = index_counter + 3;
			index_counter += 4;
		}
		std::memcpy(m_ibo->map(), ib, ibo_size);
		m_ibo->unmap();
		delete[] ib;
	}

	VKRenderer::~VKRenderer()
	{
		delete m_vbo;
	}

	void VKRenderer::begin()
	{
		m_mapped_buffer = (VertexData*)m_vbo->map();
	}

	void VKRenderer::end()
	{
		m_vbo->unmap();
	}

	void VKRenderer::submitVertex(const VertexData& vertex, int index)
	{
		m_mapped_buffer[index] = vertex;
	}

	void VKRenderer::clear()
	{
		m_buffer_pos = 0;
		m_vertex_count = 0;
	}

	void VKRenderer::render(size_t override_vertex_count) const
	{
		if (override_vertex_count > 0) {
			m_vbo->bind();
			m_ibo->bind();
			m_window->m_command_pool->drawIndices(override_vertex_count / 4 * 6);
		}
	}

}
#endif
