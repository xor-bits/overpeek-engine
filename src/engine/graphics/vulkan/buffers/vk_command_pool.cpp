#include "vk_command_pool.hpp"
#ifdef BUILD_VULKAN



namespace oe::graphics {

	VKCommandPool::VKCommandPool(const VKPhysicalDevice* physical_device, const VKLogicalDevice* logical_device)
		: m_physical_device(physical_device)
		, m_logical_device(logical_device)
	{
		QueueFamilyIndices queueFamilyIndices = m_physical_device->findQueueFamilies();

		// command pool create info
		vk::CommandPoolCreateInfo poolInfo = {};
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		// command pool creation
		m_command_pool = m_logical_device->m_logical_device.createCommandPool(poolInfo);
	}

	VKCommandPool::~VKCommandPool() {
		m_logical_device->m_logical_device.destroyCommandPool(m_command_pool);
	}



	void VKCommandPool::createCommandBuffersSwapChain(const VKSwapChain* swap_chain) {
		// command buffer creation info
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandPool = m_command_pool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = swap_chain->m_swap_chain_render_targets.size();

		// commandbuffer creation
		auto command_buffers = m_logical_device->m_logical_device.allocateCommandBuffers(allocInfo);
		for (size_t i = 0; i < swap_chain->m_swap_chain_render_targets.size(); i++)
		{
			m_command_buffers.push_back(new VKCommandBuffer(swap_chain->m_swap_chain_render_targets.at(i), command_buffers.at(i)));
		}
	}



	void VKCommandPool::beginRecording(glm::vec4 clear_color) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->beginRecording(clear_color);
		}
	}

	void VKCommandPool::endRecording() {
		for (auto& buffer : m_command_buffers)
		{
			buffer->endRecording();
		}
	}

	void VKCommandPool::bindShader(const VKShader* shader) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->bindShader(shader);
		}
	}

	void VKCommandPool::bindVertexBuffer(const VKBuffer* vertex_buffer) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->bindVertexBuffer(vertex_buffer);
		}
	}

	void VKCommandPool::bindIndexBuffer(const VKBuffer* index_buffer) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->bindIndexBuffer(index_buffer);
		}
	}
	
	void VKCommandPool::drawVertices(size_t vertex_conunt) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->drawVertices(vertex_conunt);
		}
	}

	void VKCommandPool::drawIndices(size_t index_conunt) {
		for (auto& buffer : m_command_buffers)
		{
			buffer->drawIndices(index_conunt);
		}
	}

}
#endif