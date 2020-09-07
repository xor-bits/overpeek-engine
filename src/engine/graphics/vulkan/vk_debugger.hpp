#pragma once
#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>
#include <iostream>

#include "engine/engine.hpp"



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData);

namespace oe::graphics {

	class Debugger {
	public:
		vk::DebugUtilsMessengerEXT m_debug_messenger;
		vk::DispatchLoaderDynamic m_dldy;
		
		const vk::Instance* m_instance;

	public:
		Debugger(const vk::Instance* instance);
		~Debugger();

		static vk::DebugUtilsMessengerCreateInfoEXT* createInfo() {
			vk::DebugUtilsMessengerCreateInfoEXT* createInfo = new vk::DebugUtilsMessengerCreateInfoEXT{};
			createInfo->messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose /*| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/ | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
			createInfo->messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
			createInfo->pfnUserCallback = debugCallback;
			return createInfo;
		}

	};

}



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cout << "Vulkan debug log" << std::endl;

	std::cout << fmt::format("description\t\t:\t\t{}", pCallbackData->pMessage) << std::endl;

	std::string severity = "severity\t\t:\t\t{}";
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cout << fmt::format(severity, "\u001b[34mVERBOSE\u001b[0m") << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cout << fmt::format(severity, "\u001b[33mWARNING\u001b[0m") << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cout << fmt::format(severity, "\u001b[31mERROR\u001b[0m") << std::endl;
		break;
	}

	std::string type = "type\t\t\t:\t\t{}";
	switch (messageType)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		std::cout << fmt::format(type, "GENERAL") << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		std::cout << fmt::format(type, "VALIDATION") << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		std::cout << fmt::format(type, "PERFORMANCE") << std::endl;
		break;
	default:
		break;
	}

	if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		oe_error_terminate("vulkan error");
	
	std::cout << std::endl;

	return VK_FALSE;
}
#endif