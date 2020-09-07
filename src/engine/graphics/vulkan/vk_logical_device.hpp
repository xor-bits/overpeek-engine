#pragma	once
#ifdef BUILD_VULKAN

#include "vk_physical_device.hpp"
#include "vk_support.hpp"

#include <set>



namespace oe::graphics {

	class VKLogicalDevice {
	public:
		vk::Device m_logical_device;

		vk::Queue m_graphics_queue;
		vk::Queue m_present_queue;
		
		const VKPhysicalDevice* m_physical_device;

	public:
		VKLogicalDevice(const Instance* instance, const VKPhysicalDevice* physical_device);
		~VKLogicalDevice();

	};

}
#endif