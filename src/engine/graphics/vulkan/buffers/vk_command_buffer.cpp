#include "vk_command_buffer.hpp"
#include "engine/graphics/vulkan/config.hpp"



namespace oe::graphics {

	CommandBuffer::CommandBuffer() {
		// command buffer creation info
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandPool = m_command_pool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		// commandbuffer creation
		m_command_buffer = m_logical_device->m_logical_device.allocateCommandBuffers(allocInfo);

		// commandbuffer per framebuffer
		for (size_t i = 0; i < m_command_buffers.size(); i++) {
			// command buffer begin info
			vk::CommandBufferBeginInfo beginInfo = {};
			beginInfo.pInheritanceInfo = nullptr; // Optional
			// beginInfo.flags = 0; // Optional

			// begin command buffer recording
			m_command_buffers[i].begin(beginInfo);

			// start render pass
			vk::RenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.renderPass = m_swap_chain->m_render_pass;
			renderPassInfo.framebuffer = m_swap_chain->m_swap_chain_framebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_swap_chain->m_extent;

			// clearing color
			vk::ClearColorValue clearColorValue = vk::ClearColorValue(std::array<float, 4>{ 0.2f, 0.2f, 0.2f, 1.0f });
			vk::ClearValue clearColor = {};
			clearColor.color = clearColorValue;
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			// render pass begin
			m_command_buffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

			// render pass commands
			m_command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
			vk::Buffer vertexBuffers[] = { *buffer };
			vk::DeviceSize offsets[] = { 0 };
			m_command_buffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);
			m_command_buffers[i].draw(3, 1, 0, 0);

			m_command_buffers[i].endRenderPass();

			// render pass end
			m_command_buffers[i].end();
		}
	}

	CommandBuffer::~CommandBuffer() {

	}



	void CommandBuffer::map() {

	}

	void CommandBuffer::unmap() {

	}

}