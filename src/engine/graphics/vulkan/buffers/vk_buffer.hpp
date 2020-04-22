#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/vertexData.hpp"

#include <vulkan/vulkan.hpp>



namespace oe::graphics {

	class VKLogicalDevice; class VKWindow;
	class VKBuffer {
	public:
		vk::Buffer m_staging_buffer;
		vk::DeviceMemory m_staging_buffer_memory;
		vk::Buffer m_buffer;
		vk::DeviceMemory m_buffer_memory;
		vk::BufferUsageFlagBits m_usage;

		size_t m_size;

		const VKLogicalDevice* m_logical_device;
		const VKWindow* m_window;

	public:
		VKBuffer(vk::BufferUsageFlagBits usage, size_t size, const VKLogicalDevice* logical_device, const VKWindow* window);
		~VKBuffer();

		void* map();
		void unmap();
		void bind();



		static vk::VertexInputBindingDescription getBindingDescription() {
			vk::VertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VertexData);
			bindingDescription.inputRate = vk::VertexInputRate::eVertex;
			return bindingDescription;
		}

		static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

			// position vec3
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
			attributeDescriptions[0].offset = offsetof(VertexData, position);

			// uv vec2
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = vk::Format::eR32G32Sfloat;
			attributeDescriptions[1].offset = offsetof(VertexData, position);

			// color vec4
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = vk::Format::eR32G32B32A32Sfloat;
			attributeDescriptions[2].offset = offsetof(VertexData, color);

			return attributeDescriptions;
		}

	private:
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);

	};

}
#endif