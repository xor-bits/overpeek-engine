#pragma once
#ifdef BUILD_VULKAN

#include "engine/engine.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>
#include <set>



namespace oe::graphics {

	constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
	constexpr uint32_t vk_api_version = VK_API_VERSION_1_0;

	const std::vector<const char*> required_validation_layers = {

	};

	const std::vector<const char*> required_device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		// VK_KHR_MAINTENANCE3_EXTENSION_NAME,
		// VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
		// VK_NV_RAY_TRACING_EXTENSION_NAME,
	};

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};


	std::set<std::string> getRequredLayers(bool debugging);
	bool hasRequiredValidationLayers(bool debugging);
	std::vector<const char*> getRequiredExtensions(bool debugging);

	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR* surface);

	bool hasRequiredProperties(const vk::PhysicalDevice& physical_device);
	bool hasRequiredFeatures(const vk::PhysicalDevice& physical_device);
	bool hasRequiredExtensions(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR* surface);


	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

}
#endif