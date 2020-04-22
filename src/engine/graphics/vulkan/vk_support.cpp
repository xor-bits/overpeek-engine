#include "vk_support.hpp"
#ifdef BUILD_VULKAN

#include <GLFW/glfw3.h>
#include <shaderc/shaderc.hpp>

#include "engine/engine.hpp"



namespace oe::graphics {

	SwapChainSupportDetails oe::graphics::querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR* surface) {
		SwapChainSupportDetails details;

		device.getSurfaceCapabilitiesKHR(*surface, &details.capabilities);
		details.formats = device.getSurfaceFormatsKHR(*surface);
		details.presentModes = device.getSurfacePresentModesKHR(*surface);

		return details;
	}

	std::set<std::string> oe::graphics::getRequredLayers(bool debugging) {
		std::set<std::string> requiredLayers(required_validation_layers.begin(), required_validation_layers.end());

		if (debugging) {
			requiredLayers.insert("VK_LAYER_LUNARG_standard_validation");
		}

		return requiredLayers;
	}

	bool oe::graphics::hasRequiredValidationLayers(bool debugging) {
		auto available_validation_layers = vk::enumerateInstanceLayerProperties();
		std::set<std::string> requiredLayers = getRequredLayers(debugging);

		// remove any available extensions from the list of required extensions
		for (const auto& layer : available_validation_layers) {
			requiredLayers.erase(layer.layerName);
		}

		// check if the list of required layers is empty
		if (!requiredLayers.empty()) {
			// insert the list of missing layers to this string and print it
			std::string list_of_missing_layers;
			for (const auto& layer : requiredLayers) {
				list_of_missing_layers.append("\n - " + layer);
			}

			spdlog::error("No required layer(s){}", list_of_missing_layers);
			return false;
		}
		else {
			return true;
		}
	}

	std::vector<const char*> oe::graphics::getRequiredExtensions(bool debugging) {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// vector holding extensions required by glfw and debugging if enabled
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (debugging) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			// extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		return extensions;
	}

	bool oe::graphics::hasRequiredProperties(const vk::PhysicalDevice& physical_device) {
		return true;
	}

	bool oe::graphics::hasRequiredFeatures(const vk::PhysicalDevice& physical_device) {
		return true;
	}

	bool oe::graphics::hasRequiredExtensions(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR* surface) {
		std::vector<vk::ExtensionProperties> available_extension_properties = physical_device.enumerateDeviceExtensionProperties();

		// swap chain support
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physical_device, surface);
		if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) return false;

		// check if all required features
		std::set<std::string> requiredExtensions(required_device_extensions.begin(), required_device_extensions.end());

		// remove any available extensions from the list of required extensions
		for (const auto& extension : available_extension_properties) {
			requiredExtensions.erase(extension.extensionName);
		}

		// check if the list of required extensions is empty
		if (!requiredExtensions.empty()) {
			// insert the list of missing extensions to this string and print it
			std::string list_of_missing_extensions;
			for (const auto& extension : requiredExtensions) {
				list_of_missing_extensions.append("\n - " + extension);
			}

			oe_error_terminate("No required extension(s){}", list_of_missing_extensions);
			return false;
		}
		else {
			return true;
		}
	}

	vk::SurfaceFormatKHR oe::graphics::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			// choose color format BGRA 32 bit sRGB if it is available
			if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
				return availableFormat;
			}
		}

		// if not then choose the first color format
		return availableFormats[0];
	}

	vk::PresentModeKHR oe::graphics::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
		return vk::PresentModeKHR::eImmediate; // no v-sync for debugging purposes

		for (const auto& availablePresentMode : availablePresentModes) {
			// use triple buffering if it is available
			if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
				return availablePresentMode;
			}
		}

		// if not, fall back to vertical sync
		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D oe::graphics::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
		// kind of like viewport in opengl
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			vk::Extent2D actualExtent = { actualExtent.width, actualExtent.height };

			// clamp extent size to capabilities.xxxImageExtent
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

}
#endif