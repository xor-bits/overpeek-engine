#include "vk_buffer.hpp"
#ifdef BUILD_VULKAN

#include "engine/graphics/vulkan/vk_logical_device.hpp"
#include "engine/graphics/vulkan/vk_window.hpp"
#include "engine/graphics/vulkan/buffers/vk_command_pool.hpp"



namespace oe::graphics {

	VKBuffer::VKBuffer(vk::BufferUsageFlagBits usage, size_t size, const VKLogicalDevice* logical_device, const VKWindow* window)
		: m_usage(usage)
		, m_logical_device(logical_device)
		, m_window(window)
		, m_size(size)
	{
		// staging buffer
		{
			vk::BufferCreateInfo bufferInfo = {};
			bufferInfo.size = size;
			bufferInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
			bufferInfo.sharingMode = vk::SharingMode::eExclusive;
			m_staging_buffer = m_logical_device->m_logical_device.createBuffer(bufferInfo);

			// memory
			vk::MemoryRequirements memRequirements = m_logical_device->m_logical_device.getBufferMemoryRequirements(m_staging_buffer);
			vk::MemoryAllocateInfo allocInfo = {};
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
			m_staging_buffer_memory = m_logical_device->m_logical_device.allocateMemory(allocInfo);
			m_logical_device->m_logical_device.bindBufferMemory(m_staging_buffer, m_staging_buffer_memory, 0);
		}

		// buffer
		{
			vk::BufferCreateInfo bufferInfo = {};
			bufferInfo.size = size;
			bufferInfo.usage = vk::BufferUsageFlagBits::eTransferDst | usage;
			bufferInfo.sharingMode = vk::SharingMode::eExclusive;
			m_buffer = m_logical_device->m_logical_device.createBuffer(bufferInfo);

			// memory
			vk::MemoryRequirements memRequirements = m_logical_device->m_logical_device.getBufferMemoryRequirements(m_buffer);
			vk::MemoryAllocateInfo allocInfo = {};
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
			m_buffer_memory = m_logical_device->m_logical_device.allocateMemory(allocInfo);
			m_logical_device->m_logical_device.bindBufferMemory(m_buffer, m_buffer_memory, 0);
		}
	}

	VKBuffer::~VKBuffer() {
		m_logical_device->m_logical_device.destroyBuffer(m_buffer);
		m_logical_device->m_logical_device.freeMemory(m_buffer_memory);
		m_logical_device->m_logical_device.destroyBuffer(m_staging_buffer);
		m_logical_device->m_logical_device.freeMemory(m_staging_buffer_memory);
	}

	void copyBuffer(const VKWindow* window, vk::Buffer src, vk::Buffer dst, size_t size) {
		// copy staging buffer to main buffer
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandPool = window->m_command_pool->m_command_pool;
		allocInfo.commandBufferCount = 1;

		vk::CommandBuffer command_buffer;
		window->m_logical_device->m_logical_device.allocateCommandBuffers(&allocInfo, &command_buffer);

		vk::CommandBufferBeginInfo begin_info = {};
		begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		command_buffer.begin(begin_info);
		{
			vk::BufferCopy copy_region = {};
			copy_region.srcOffset = 0; // Optional
			copy_region.dstOffset = 0; // Optional
			copy_region.size = size;
			command_buffer.copyBuffer(src, dst, 1, &copy_region);
		}
		command_buffer.end();

		vk::SubmitInfo submit_info = {};
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;
		window->m_logical_device->m_graphics_queue.submit(1, &submit_info, nullptr);
		window->m_logical_device->m_graphics_queue.waitIdle();

		window->m_logical_device->m_logical_device.freeCommandBuffers(window->m_command_pool->m_command_pool, 1, &command_buffer);
	}

	void* VKBuffer::map() {
		return m_logical_device->m_logical_device.mapMemory(m_staging_buffer_memory, 0, m_size);
	}

	void VKBuffer::unmap() {
		m_logical_device->m_logical_device.unmapMemory(m_staging_buffer_memory);

		copyBuffer(m_window, m_staging_buffer, m_buffer, m_size);
	}

	void VKBuffer::bind() {
		switch (m_usage)
		{
		case vk::BufferUsageFlagBits::eVertexBuffer:
			m_window->m_command_pool->bindVertexBuffer(this);
			break;
		case vk::BufferUsageFlagBits::eIndexBuffer:
			m_window->m_command_pool->bindIndexBuffer(this);
			break;
		default:
			break;
		}
	}

	uint32_t VKBuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
		vk::PhysicalDeviceMemoryProperties memProperties = m_logical_device->m_physical_device->m_physical_device.getMemoryProperties();

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		spdlog::error("failed to find suitable memory type");
	}

}
#endif