#pragma	once
#ifdef BUILD_VULKAN

#include "vk_logical_device.hpp"
#include "vk_physical_device.hpp"
#include "vk_support.hpp"

#include <set>



namespace oe::graphics {
	
	VKLogicalDevice::VKLogicalDevice(const Instance* instance, const VKPhysicalDevice* physical_device)
		: m_physical_device(physical_device) 
	{
		auto indices = m_physical_device->findQueueFamilies();

		// all queues
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			vk::DeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		// used features
		vk::PhysicalDeviceFeatures deviceFeatures = {};

		// logical device creation info
		vk::DeviceCreateInfo createInfo = {};
		createInfo.pEnabledFeatures = &deviceFeatures;
		// multiple queues
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		// device extensions
		createInfo.enabledExtensionCount = static_cast<uint32_t>(required_device_extensions.size());
		createInfo.ppEnabledExtensionNames = required_device_extensions.data();

		// device layers
		if (instance->m_instance_info.debug_messages) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(required_validation_layers.size());
			createInfo.ppEnabledLayerNames = required_validation_layers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		// logical device creation
		vk::Result vk_result = m_physical_device->m_physical_device.createDevice(&createInfo, nullptr, &m_logical_device);
		if (vk_result != vk::Result::eSuccess) {
			spdlog::error("Vulkan logical device creation failed ({})", vk_result);
		}

		m_graphics_queue = m_logical_device.getQueue(indices.graphicsFamily.value(), 0);
		m_present_queue = m_logical_device.getQueue(indices.presentFamily.value(), 0);
	}

	VKLogicalDevice::~VKLogicalDevice() {
		m_logical_device.destroy();
	}

}
#endif