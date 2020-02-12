#include "vk_vertex_buffer.hpp"
#include "engine/graphics/vulkan/vk_logical_device.hpp"



namespace oe::graphics {

	VertexBuffer::VertexBuffer(vk::BufferUsageFlagBits usage, size_t size, const LogicalDevice* logical_device)
		: m_logical_device(logical_device), m_size(size) 
	{
		// vertex buffer
		vk::BufferCreateInfo bufferInfo = {};
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = vk::SharingMode::eExclusive;
		m_vertex_buffer = m_logical_device->m_logical_device.createBuffer(bufferInfo);

		// memory
		vk::MemoryRequirements memRequirements = m_logical_device->m_logical_device.getBufferMemoryRequirements(m_vertex_buffer);
		vk::MemoryAllocateInfo allocInfo = {};
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		m_vertex_buffer_memory = m_logical_device->m_logical_device.allocateMemory(allocInfo);
	}

	VertexBuffer::~VertexBuffer() {
		m_logical_device->m_logical_device.destroyBuffer(m_vertex_buffer);
		m_logical_device->m_logical_device.freeMemory(m_vertex_buffer_memory);
	}

	void VertexBuffer::map() {
		m_logical_device->m_logical_device.bindBufferMemory(m_vertex_buffer, m_vertex_buffer_memory, 0);
		mapped_buffer = m_logical_device->m_logical_device.mapMemory(m_vertex_buffer_memory, 0, m_size);
	}

	void VertexBuffer::unmap() {
		m_logical_device->m_logical_device.unmapMemory(m_vertex_buffer_memory);
	}
	
	void VertexBuffer::submit(const VertexData& vertex_data) {
		std::memcpy(mapped_buffer, &vertex_data, sizeof(vertex_data));
	}

	uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
		vk::PhysicalDeviceMemoryProperties memProperties = m_logical_device->m_physical_device->m_physical_device.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		spdlog::error("failed to find suitable memory type");
	}

}