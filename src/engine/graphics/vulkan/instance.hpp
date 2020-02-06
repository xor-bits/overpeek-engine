#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../config.hpp"
#include "vulkan_support.hpp"
#include "debugger.hpp"




namespace oe::vulkan {

	constexpr uint32_t vk_api_version = VK_API_VERSION_1_0;

	class Instance {
	public:
		vk::Instance m_instance;
		std::vector<vk::LayerProperties> m_available_validation_layers;

		const bool m_debugging;
		Debugger *m_debugger;

	public:
		Instance(bool debugging = true) : m_debugging(debugging) {
			// check included validation layers
			hasRequiredValidationLayers(m_debugging);

			// application info
			vk::ApplicationInfo appInfo = {};
			appInfo.pApplicationName = "Vulkan Test";
			appInfo.applicationVersion = vk_api_version;
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = vk_api_version;
			appInfo.apiVersion = vk_api_version;

			// glfw extensions
			auto extensions = getRequiredExtensions(m_debugging);

			// instance info
			vk::InstanceCreateInfo createInfo = {};
			createInfo.enabledExtensionCount = extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();
			createInfo.pApplicationInfo = &appInfo;

			// instance validation layers
			std::set<std::string> validation_layers_set = getRequredLayers(m_debugging);
			std::vector<const char*> validation_layers;
			for (const std::string& str : validation_layers_set) { validation_layers.push_back(str.c_str()); }
			auto debugCreateInfo = Debugger::createInfo();
			if (m_debugging) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
				createInfo.ppEnabledLayerNames = validation_layers.data();
				createInfo.pNext = debugCreateInfo;
			}
			else { // no debugging
				createInfo.enabledLayerCount = 0;
				createInfo.pNext = nullptr;
			}

			// instance creation
			m_instance = vk::createInstance(createInfo);
			
			delete debugCreateInfo;

			// debugger
			m_debugger = nullptr;
			if (m_debugging) {
				m_debugger = new Debugger(&m_instance);
			}
		}

		~Instance() {
			// debugger
			if (m_debugging) {
				delete m_debugger;
			}

			m_instance.destroy();
		}

	};

}