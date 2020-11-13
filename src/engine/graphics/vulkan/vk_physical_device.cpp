#include "vk_physical_device.hpp"
#ifdef BUILD_VULKAN
#include "vk_support.hpp"
#include "engine/engine.hpp"

#include <map>
#include <set>
#include <iostream>



namespace oe::graphics {

	VKPhysicalDevice::VKPhysicalDevice(const VKInstance* instance, const vk::SurfaceKHR* surface)
		: m_surface(surface), m_instance(instance)
	{
		pickPhysicalDevice();
	}

	VKPhysicalDevice::~VKPhysicalDevice() {

	}

	QueueFamilyIndices VKPhysicalDevice::findQueueFamilies() const {
		return findQueueFamilies(m_physical_device, m_surface);
	}

	QueueFamilyIndices VKPhysicalDevice::findQueueFamilies(vk::PhysicalDevice device, const vk::SurfaceKHR* surface) {
		QueueFamilyIndices queue_family_indices = {};

		std::vector<vk::QueueFamilyProperties> available_queue_families = device.getQueueFamilyProperties();
		for (size_t i = 0; i < available_queue_families.size(); i++) {
			// graphics queue
			if (available_queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queue_family_indices.graphicsFamily = i;
			}

			// surface support
			vk::Bool32 presentSupport = false;
			device.getSurfaceSupportKHR(i, *surface, &presentSupport);
			if (presentSupport) {
				queue_family_indices.presentFamily = i;
			}
		}

		return queue_family_indices;
	}

	bool VKPhysicalDevice::isDeviceSuitable(vk::PhysicalDevice device) {
		QueueFamilyIndices queue_family_indices = findQueueFamilies(device, m_surface);

		bool features = hasRequiredFeatures(device);
		bool properties = hasRequiredProperties(device);
		bool extensions = hasRequiredExtensions(device, m_surface);

		return queue_family_indices.isComplete() && extensions && features;
	}

	int VKPhysicalDevice::rateDeviceSuitability(vk::PhysicalDevice device) {
		// assign score to all GPUs
		int score = 1;

		if (!isDeviceSuitable(device)) return 0;

		// Discrete GPUs have a significant performance advantage
		if (m_instance->m_instance_info.favored_gpu_vulkan == oe::gpu::integrated) {
			if (device.getProperties().deviceType == vk::PhysicalDeviceType::eIntegratedGpu) {
				score += 1;
			}
		}
		else { // dedicated
			if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
				score += 1;
			}
		}

		return score;
	}

	vk::PhysicalDevice VKPhysicalDevice::bestPhysicalDevice(const std::vector<vk::PhysicalDevice>& devices) {
		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, VkPhysicalDevice> candidates;
		for (const auto& device : devices) {
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// count all suitable GPUs
		size_t suitable = 0;
		for (auto gpu : candidates)
			if (gpu.first > 0) suitable++;

		// check if any
		if (suitable == 0)
			throw std::runtime_error("No suitable GPUs");

		// check if the best GPU is even suitable
		auto best = candidates.rbegin();
		if (best->first > 0)
			return best->second;
	}

	void VKPhysicalDevice::pickPhysicalDevice() {
		// all GPUs with Vulkan support
		uint32_t deviceCount = 0;

		std::vector<vk::PhysicalDevice> devices = m_instance->m_vk_instance.enumeratePhysicalDevices();
		if (devices.size() == 0)
			throw std::runtime_error("No GPUs with Vulkan support");

		// select GPU
		m_physical_device = bestPhysicalDevice(devices);
		m_physical_device_properties = m_physical_device.getProperties();
		spdlog::debug("selected gpu = {}", m_physical_device_properties.deviceName);
	}

}
#endif