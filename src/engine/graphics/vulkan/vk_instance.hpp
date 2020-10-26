#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/interface/instance.hpp"

#include <vulkan/vulkan.hpp>

// Special thanks for this repository for teaching Vulkan
// https://github.com/Overv/VulkanTutorial



namespace oe::graphics {

	class VKPhysicalDevice; class Debugger;

	class VKInstance : public Instance {
	public:
		vk::Instance m_vk_instance;
		std::vector<vk::LayerProperties> m_available_validation_layers;
		Debugger *m_debugger;
		VKPhysicalDevice *m_physical_device;

	public:
		VKInstance(const InstanceInfo& instance_info);
		~VKInstance();

		void vulkanInstance();

		// Inherited via Instance
		Window* createWindow(const WindowInfo& window_config) const override;
		void destroyWindow(Window* window) const override;

	public:
		// Inherited via Instance
		void blending(oe::modes mode = oe::modes::enable) const override;
		void depth(depth_functions func = depth_functions::always) const override;
		void swapInterval(unsigned int interval = 0) const override;
		void culling(culling_modes c = culling_modes::neither) const override;
		void lineWidth(float w = 1.0f) const override;
		void pointRadius(float w = 1.0f) const override;
		void polygonMode(polygon_mode p = polygon_mode::fill) const override;

	};

}
#endif