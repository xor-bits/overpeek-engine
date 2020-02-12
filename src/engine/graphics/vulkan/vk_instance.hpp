#pragma once

#include "engine/graphics/interface/instance.hpp"

#include <vulkan/vulkan.hpp>

// Special thanks for this repository for teaching Vulkan
// https://github.com/Overv/VulkanTutorial



namespace oe::graphics {

	class PhysicalDevice; class Debugger;

	class VKInstance : public Instance {
	public:
		vk::Instance m_vk_instance;
		std::vector<vk::LayerProperties> m_available_validation_layers;
		Debugger *m_debugger;
		PhysicalDevice *m_physical_device;

	public:
		VKInstance(const InstanceInfo& instance_info);
		~VKInstance();

		void vulkanInstance();

		// Inherited via Instance
		virtual Window* createWindow(const WindowInfo& window_config) const override;
		virtual void destroyWindow(Window* window) const override;

	public:
		// Inherited via Instance
		virtual void blending(oe::modes mode = oe::modes::enable) const override;
		virtual void depth(depth_functions func = depth_functions::always) const override;
		virtual void swapInterval(unsigned int interval = 0) const override;
		virtual void culling(culling_modes c = culling_modes::neither) const override;
		virtual void lineWidth(float w = 1.0f) const override;
		virtual void pointRadius(float w = 1.0f) const override;
		virtual void polygonMode(polygon_mode p = polygon_mode::fill) const override;

	};

}