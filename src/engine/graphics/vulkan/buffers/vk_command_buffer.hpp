#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/vulkan/vk_shader.hpp"
#include "engine/graphics/vulkan/vk_physical_device.hpp"
#include "engine/graphics/vulkan/vk_logical_device.hpp"
#include "engine/graphics/vulkan/vk_render_target.hpp"
#include "vk_buffer.hpp"

#include <vulkan/vulkan.hpp>



namespace oe::graphics {

	class VKCommandPool;
	class VKCommandBuffer {
	public:
		vk::CommandBuffer m_command_buffer;

		vk::CommandBufferBeginInfo beginInfo;
		vk::RenderPassBeginInfo renderPassInfo;

		const VKPhysicalDevice* m_physical_device;
		const VKLogicalDevice* m_logical_device;

	public:
		VKCommandBuffer(VKRenderTarget* render_target, vk::CommandBuffer command_buffer);
		~VKCommandBuffer();

		void beginRecording(glm::vec4 clear_color = oe::colors::clear_color);
		void endRecording();

		void bindShader(const VKShader* shader);
		void bindVertexBuffer(const VKBuffer* vertex_buffer);
		void bindIndexBuffer(const VKBuffer* index_buffer);
		void drawVertices(size_t vertex_conunt);
		void drawIndices(size_t index_conunt);

	};

}
#endif