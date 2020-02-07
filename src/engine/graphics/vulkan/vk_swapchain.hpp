#pragma once

#include "logical_device.hpp"
#include "window.hpp"



namespace oe::vulkan {

	class SwapChain {
	public:
		vk::RenderPass m_render_pass;

		vk::SwapchainKHR m_swap_chain;
		std::vector<vk::Image> m_swap_chain_images;
		std::vector<vk::ImageView> m_swap_chain_image_views;
		std::vector<vk::Framebuffer> m_swap_chain_framebuffers;

		vk::Format m_swap_chain_image_format;
		vk::Extent2D m_extent;

		const LogicalDevice* m_logical_device;
		const PhysicalDevice* m_physical_device;
		const Window* m_window;

	public:
		SwapChain(const LogicalDevice* logical_device) : m_logical_device(logical_device), m_physical_device(logical_device->m_physical_device), m_window(logical_device->m_physical_device->m_window) {
			// get swap chain support
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physical_device->m_physical_device, m_window->m_surface);

			// set up everything
			vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			m_extent = chooseSwapExtent(swapChainSupport.capabilities);

			// set swap chain
			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			// swap chain object info
			vk::SwapchainCreateInfoKHR createInfo = {};
			createInfo.surface = m_window->m_surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = m_extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

			// queues
			PhysicalDevice::QueueFamilyIndices indices = m_physical_device->findQueueFamilies();
			uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
			if (indices.graphicsFamily != indices.presentFamily) {
				createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				createInfo.imageSharingMode = vk::SharingMode::eExclusive;
				createInfo.queueFamilyIndexCount = 0; // Optional
				createInfo.pQueueFamilyIndices = nullptr; // Optional
			}

			// no transform
			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

			// no window transparency
			createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE; // normal window, not frontmost window

			// only after resize
			createInfo.oldSwapchain = nullptr;

			// swap chain creation
			vk::Result res = m_logical_device->m_logical_device.createSwapchainKHR(&createInfo, nullptr, &m_swap_chain);
			if (res != vk::Result::eSuccess) {
				spdlog::error("failed to create swap chain");
			}

			// retrive handles to swapchain images
			m_swap_chain_images = m_logical_device->m_logical_device.getSwapchainImagesKHR(m_swap_chain);

			// store format
			m_swap_chain_image_format = surfaceFormat.format;

			createImageViews();
			createRenderPass();
		}

		void createRenderPass() {
			vk::AttachmentDescription colorAttachment = {};
			colorAttachment.format = m_swap_chain_image_format;
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
			m_render_pass = m_logical_device->m_logical_device.createRenderPass(renderPassInfo);

			// finish swap chain with this render pass
			createFramebuffers();
		}

		~SwapChain() {
			for (auto framebuffer : m_swap_chain_framebuffers) {
				m_logical_device->m_logical_device.destroyFramebuffer(framebuffer);
			}

			m_logical_device->m_logical_device.destroyRenderPass(m_render_pass);

			// all image views
			for (auto imageView : m_swap_chain_image_views) {
				m_logical_device->m_logical_device.destroyImageView(imageView);
			}

			// swap chain
			m_logical_device->m_logical_device.destroySwapchainKHR(m_swap_chain);
		}

		void createImageViews() {
			// image views for images in swap chain
			m_swap_chain_image_views.resize(m_swap_chain_images.size());

			// all swap chain images
			for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
				// basic image view create info
				vk::ImageViewCreateInfo createInfo = {};
				createInfo.image = m_swap_chain_images[i];

				// 2d images
				createInfo.viewType = vk::ImageViewType::e2D;
				createInfo.format = m_swap_chain_image_format;

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
				m_swap_chain_image_views[i] = m_logical_device->m_logical_device.createImageView(createInfo);
			}
		}

		void createFramebuffers() {
			m_swap_chain_framebuffers.resize(m_swap_chain_image_views.size());

			// create framebuffer for every swap chain image
			for (size_t i = 0; i < m_swap_chain_framebuffers.size(); i++) {
				vk::ImageView attachments[] = {
					m_swap_chain_image_views[i]
				};

				// framebuffer create info
				vk::FramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.renderPass = m_render_pass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = m_extent.width;
				framebufferInfo.height = m_extent.height;
				framebufferInfo.layers = 1;

				// framebuffer creation
				m_swap_chain_framebuffers[i] = m_logical_device->m_logical_device.createFramebuffer(framebufferInfo);
			}
		}

	};

}