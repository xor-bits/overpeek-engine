#include "vk_instance.hpp"

#include "config.hpp"
#include "vk_support.hpp"
#include "vk_window.hpp"
#include "vk_physical_device.hpp"
#include "vk_debugger.hpp"



namespace oe::graphics {

	VKInstance::VKInstance(const InstanceInfo& instance_info) 
		: Instance(instance_info)
	{
		vulkanInstance();
	}

	VKInstance::~VKInstance() {
		// debugger
		if (m_instance_info.debug_messages) {
			delete m_debugger;
		}

		m_vk_instance.destroy();
	}

	void VKInstance::vulkanInstance() {
		// check included validation layers
		hasRequiredValidationLayers(m_instance_info.debug_messages);

		// application info
		vk::ApplicationInfo appInfo = {};
		appInfo.pApplicationName = "overpeek-engine";
		appInfo.applicationVersion = vk_api_version;
		appInfo.pEngineName = "overpeek-engine";
		appInfo.engineVersion = vk_api_version;
		appInfo.apiVersion = vk_api_version;

		// glfw extensions
		auto extensions = getRequiredExtensions(m_instance_info.debug_messages);

		// instance info
		vk::InstanceCreateInfo createInfo = {};
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.pApplicationInfo = &appInfo;

		// instance validation layers
		std::set<std::string> validation_layers_set = getRequredLayers(m_instance_info.debug_messages);
		std::vector<const char*> validation_layers;
		for (const std::string& str : validation_layers_set) { validation_layers.push_back(str.c_str()); }
		auto debugCreateInfo = Debugger::createInfo();
		if (m_instance_info.debug_messages) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
			createInfo.ppEnabledLayerNames = validation_layers.data();
			createInfo.pNext = debugCreateInfo;
		}
		else { // no debugging
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		// instance creation
		m_vk_instance = vk::createInstance(createInfo);

		delete debugCreateInfo;

		// debugger
		m_debugger = nullptr;
		if (m_instance_info.debug_messages) {
			m_debugger = new Debugger(&m_vk_instance);
		}
	}



	Window* VKInstance::createWindow(const WindowInfo& window_config) const
	{
		return new VKWindow(this, window_config);
	}

	void VKInstance::destroyWindow(Window* window) const
	{
		delete window;
	}

	void VKInstance::blending(oe::modes mode) const
	{
	}

	void VKInstance::depth(depth_functions func) const
	{
	}

	void VKInstance::swapInterval(unsigned int interval) const
	{
	}

	void VKInstance::culling(culling_modes c) const
	{
	}

	void VKInstance::lineWidth(float w) const
	{
	}

	void VKInstance::pointRadius(float w) const
	{
	}

	void VKInstance::polygonMode(polygon_mode p) const
	{
	}

}