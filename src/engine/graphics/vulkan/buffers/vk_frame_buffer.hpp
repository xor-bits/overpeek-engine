#pragma once

#include "engine/graphics/vulkan/vk_logical_device.hpp"
#include "engine/graphics/vertexData.hpp"



namespace oe::graphics {

	class FrameBuffer {
	public:
		vk::Image m_image;
		vk::ImageView m_image_view;
		vk::Framebuffer m_framebuffer;

		vk::Extent2D m_extent;

		const LogicalDevice* m_logical_device;

	public:
		FrameBuffer(vk::Image m_image);
		FrameBuffer(const glm::vec2& size);
		~FrameBuffer();

	};

}