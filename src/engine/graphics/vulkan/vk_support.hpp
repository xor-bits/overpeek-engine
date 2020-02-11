#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>
#include <set>



namespace oe::graphics {

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};


	std::set<std::string> getRequredLayers(bool debugging);
	bool hasRequiredValidationLayers(bool debugging);
	std::vector<const char*> getRequiredExtensions(bool debugging);

	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

	bool hasRequiredProperties(const vk::PhysicalDevice& physical_device);
	bool hasRequiredFeatures(const vk::PhysicalDevice& physical_device);
	bool hasRequiredExtensions(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface);


	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
	vk::ShaderModule createShaderModule(vk::Device logical_device, const std::vector<char>& code);

}