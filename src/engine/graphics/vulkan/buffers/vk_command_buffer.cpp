#include "vk_command_buffer.hpp"
#ifdef BUILD_VULKAN



namespace oe::graphics {

	VKCommandBuffer::VKCommandBuffer(VKRenderTarget* render_target, vk::CommandBuffer command_buffer)
		: m_command_buffer(command_buffer)
	{
		beginInfo = vk::CommandBufferBeginInfo();
		renderPassInfo = vk::RenderPassBeginInfo();

		// command buffer begin info
		beginInfo.pInheritanceInfo = nullptr; // Optional
		// beginInfo.flags = 0; // Optional

		// start render pass
		renderPassInfo.renderPass = *render_target->m_render_pass;
		renderPassInfo.framebuffer = render_target->m_frame_buffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = render_target->m_extent;
	}

	VKCommandBuffer::~VKCommandBuffer() {

	}



	void VKCommandBuffer::beginRecording(oe::color clear_color)
	{
		// clearing color
		vk::ClearColorValue clearColorValue = vk::ClearColorValue(std::array<float, 4>{ clear_color.r, clear_color.g, clear_color.b, clear_color.a });
		vk::ClearValue clearColor = {};
		clearColor.color = clearColorValue;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		// begin command buffer recording
		m_command_buffer.begin(beginInfo);
		m_command_buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	}

	void VKCommandBuffer::endRecording()
	{
		m_command_buffer.endRenderPass();
		m_command_buffer.end();
	}

	void VKCommandBuffer::bindShader(const VKShader* shader) {
		m_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, shader->m_pipeline); // TODO: make pipeline bind point defined in shader itself
	}
	
	void VKCommandBuffer::bindVertexBuffer(const VKBuffer* vertex_buffer) {
		vk::Buffer vertexBuffers[] = { vertex_buffer->m_buffer };
		vk::DeviceSize offsets[] = { 0 };
		m_command_buffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
	}

	void VKCommandBuffer::bindIndexBuffer(const VKBuffer* index_buffer) {
		m_command_buffer.bindIndexBuffer(index_buffer->m_buffer, 0, vk::IndexType::eUint16);
	}
	
	void VKCommandBuffer::drawVertices(size_t vertex_conunt) {
		m_command_buffer.draw(vertex_conunt, 1, 0, 0);
	}

	void VKCommandBuffer::drawIndices(size_t index_conunt) {
		m_command_buffer.drawIndexed(index_conunt, 1, 0, 0, 0);
	}

}
#endif