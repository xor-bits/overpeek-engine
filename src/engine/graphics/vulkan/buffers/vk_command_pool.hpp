#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/vulkan/vk_swapchain.hpp"
#include "vk_command_buffer.hpp"



namespace oe::graphics {

	class VKCommandPool {
	public:
		vk::CommandPool m_command_pool;
		std::vector<VKCommandBuffer*> m_command_buffers;

		const VKPhysicalDevice* m_physical_device;
		const VKLogicalDevice* m_logical_device;

	public:
		VKCommandPool(const VKPhysicalDevice* physical_device, const VKLogicalDevice* logical_device);
		~VKCommandPool();

		// insert or remove command buffers, or what ever
		// inline std::vector<VKCommandBuffer*>& commandBuffers() { return m_command_buffers; };
		void createCommandBuffersSwapChain(const VKSwapChain* swap_chain);

		// must be recording while submitting commands
		// must not be recording while submitting the queue
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