#pragma once

#include "engine/graphics/vulkan/config.hpp"
#include "engine/graphics/vulkan/vk_shader.hpp"
#include "engine/graphics/vulkan/vk_physical_device.hpp"
#include "engine/graphics/vulkan/buffers/vk_vertex_buffer.hpp"



namespace oe::graphics {

	class CommandPool;
	class CommandBuffer {
	public:
		vk::CommandBuffer m_command_buffer;

		const PhysicalDevice* m_physical_device;
		const LogicalDevice* m_logical_device;

	public:
		CommandBuffer();
		~CommandBuffer();

		void begin(glm::vec4 clear_color = oe::colors::clear_color);
		void end();
		
		void draw();

	};

}