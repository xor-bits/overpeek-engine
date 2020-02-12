#pragma once

#include "vk_logical_device.hpp"
#include "vk_window.hpp"
#include "vk_support.hpp"
#include "vk_render_target.hpp"



namespace oe::graphics {

	class SwapChain {
	public:
		vk::SwapchainKHR m_swap_chain;
		vk::Format m_format;
		vk::Extent2D m_extent;
		vk::RenderPass m_render_pass;

		std::vector<RenderTarget> m_swap_chain_render_targets;

		const LogicalDevice* m_logical_device;
		const PhysicalDevice* m_physical_device;

	public:
		SwapChain(const VKWindow* window, const LogicalDevice* logical_device) : m_logical_device(logical_device), m_physical_device(logical_device->m_physical_device) {
			// get swap chain support
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physical_device->m_physical_device, &window->m_surface);

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
			createInfo.surface = window->m_surface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = m_extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

			// queues
			QueueFamilyIndices indices = m_physical_device->findQueueFamilies();
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
			m_format = surfaceFormat.format;
			auto swap_chain_images = m_logical_device->m_logical_device.getSwapchainImagesKHR(m_swap_chain);
			
			createRenderPass();
			for (auto& image : swap_chain_images)
			{
				m_swap_chain_render_targets.push_back(RenderTarget(m_logical_device, &m_render_pass, image, m_extent, m_format));
			}
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
			m_render_pass = m_logical_device->m_logical_device.createRenderPass(renderPassInfo);
		}

	};

}