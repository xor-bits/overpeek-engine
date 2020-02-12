#pragma once

#include "vk_logical_device.hpp"

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>



namespace oe::graphics {

	class RenderTarget {
	public:
		vk::Extent2D m_extent;
		vk::Format m_format;
		vk::RenderPass* m_render_pass;
		bool external_render_pass;

		vk::Image m_image;
		vk::ImageView m_image_view;
		vk::Framebuffer m_frame_buffer;

		const LogicalDevice* m_logical_device;

	public:
		RenderTarget(const LogicalDevice* logical_device, const glm::vec2& size)
			: m_logical_device(logical_device)
			, m_extent(size.x, size.y)
			, m_format(vk::Format::eR8G8B8A8Srgb)
			, external_render_pass(false)
		{
			createImage();
			createImageView();
			createRenderPass();
			createFramebuffer();
		}

		// this will take the controll over vk:Image image
		// and will destroy it when needed to
		RenderTarget(const LogicalDevice* logical_device, vk::RenderPass* render_pass, vk::Image image, vk::Extent2D extent, vk::Format format)
			: m_logical_device(logical_device)
			, m_image(image)
			, m_extent(extent)
			, m_format(format)
			, m_render_pass(render_pass)
			, external_render_pass(true)
		{
			createImageView();
			createFramebuffer();
		}

		~RenderTarget() {
			m_logical_device->m_logical_device.destroyFramebuffer(m_frame_buffer);
			if (!external_render_pass) m_logical_device->m_logical_device.destroyRenderPass(*m_render_pass);
			m_logical_device->m_logical_device.destroyImageView(m_image_view);
			m_logical_device->m_logical_device.destroyImage(m_image);
		}

		void createImage() {
			vk::ImageCreateInfo image_info;
			image_info.extent.width = m_extent.width;
			image_info.extent.height = m_extent.height;
			image_info.extent.depth = 1;
			image_info.imageType = vk::ImageType::e2D;
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.format = m_format;

			m_image = m_logical_device->m_logical_device.createImage(image_info);
		}

		void createImageView() {
			// basic image view create info
			vk::ImageViewCreateInfo createInfo = {};
			createInfo.image = m_image;

			// 2d images
			createInfo.viewType = vk::ImageViewType::e2D;
			createInfo.format = m_format;

			// default color mapping
			createInfo.components.r = vk::ComponentSwizzle::eIdentity;
			createInfo.components.g = vk::ComponentSwizzle::eIdentity;
			createInfo.components.b = vk::ComponentSwizzle::eIdentity;
			createInfo.components.a = vk::ComponentSwizzle::eIdentity;

			// image usage
			createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			// image view creation
			m_image_view = m_logical_device->m_logical_device.createImageView(createInfo);
		}

		void createRenderPass() {
			vk::AttachmentDescription colorAttachment = {};
			colorAttachment.format = m_format;
			colorAttachment.samples = vk::SampleCountFlagBits::e1; // no multisampling
			colorAttachment.loadOp = vk::AttachmentLoadOp::eClear; // ... color attachment before render
			colorAttachment.storeOp = vk::AttachmentStoreOp::eStore; // store color attachment after render

			// not using stencil buffer for now
			colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
			colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

			// no idea
			colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
			colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

			// render subpasses (1)
			// attachment refrence
			vk::AttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0; // fragment shader layout color attachment
			colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

			// subpass
			vk::SubpassDescription subpass = {};
			subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			// subpass dependency
			vk::SubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			dependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead; // might fail, was 0
			dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

			// render pass create info
			vk::RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			// render pass creation
			m_render_pass = &m_logical_device->m_logical_device.createRenderPass(renderPassInfo);
		}

		void createFramebuffer() {
			vk::ImageView attachments[] = {
				m_image_view
			};

			// framebuffer create info
			vk::FramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.renderPass = *m_render_pass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_extent.width;
			framebufferInfo.height = m_extent.height;
			framebufferInfo.layers = 1;

			// framebuffer creation
			m_frame_buffer = m_logical_device->m_logical_device.createFramebuffer(framebufferInfo);
		}

	};

}