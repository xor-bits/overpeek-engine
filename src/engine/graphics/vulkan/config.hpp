#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>



constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;
constexpr uint32_t vk_api_version = VK_API_VERSION_1_0;

const std::vector<const char*> required_validation_layers = {
	
};

const std::vector<const char*> required_device_extensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};