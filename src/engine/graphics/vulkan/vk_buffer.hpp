#pragma once

#include "config.hpp"

#include <glm/glm.hpp>



namespace oe::vulkan {

	struct VertexData {
		glm::vec2 position;
		glm::vec3 color;

		static vk::VertexInputBindingDescription getBindingDescription() {
			vk::VertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VertexData);
			bindingDescription.inputRate = vk::VertexInputRate::eVertex;
			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};

			// position
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
			attributeDescriptions[0].offset = offsetof(VertexData, position);

			// color
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[1].offset = offsetof(VertexData, color);

			return attributeDescriptions;
		}
	};

	class Buffer {
	public:
		vk::Buffer m_vertex_buffer;
		vk::DeviceMemory m_vertex_buffer_memory;

		size_t m_size;

		const LogicalDevice* m_logical_device;

	public:
		Buffer(vk::BufferUsageFlagBits usage, size_t size, const LogicalDevice* logical_device) : m_logical_device(logical_device), m_size(size) {
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
			allocInfo.memoryTypeIndex = findMemoryType(m_logical_device->m_physical_device, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
			m_vertex_buffer_memory = m_logical_device->m_logical_device.allocateMemory(allocInfo);
		}

		~Buffer() {
			m_logical_device->m_logical_device.destroyBuffer(m_vertex_buffer);
			m_logical_device->m_logical_device.freeMemory(m_vertex_buffer_memory);
		}

		void setData(const void* data, size_t size, size_t offset) {
			m_logical_device->m_logical_device.bindBufferMemory(m_vertex_buffer, m_vertex_buffer_memory, 0);
			
			void* mapped_buffer;
			mapped_buffer = m_logical_device->m_logical_device.mapMemory(m_vertex_buffer_memory, offset, size);
			std::memcpy(mapped_buffer, data, size);
			m_logical_device->m_logical_device.unmapMemory(m_vertex_buffer_memory);
		}

	private:
		static uint32_t findMemoryType(const PhysicalDevice* physical_device, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
			vk::PhysicalDeviceMemoryProperties memProperties = physical_device->m_physical_device.getMemoryProperties();

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			spdlog::error("failed to find suitable memory type");
		}

	};

}