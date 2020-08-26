#include "vk_debugger.hpp"
#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>



namespace oe::graphics {

	Debugger::Debugger(const vk::Instance* instance) 
		: m_instance(instance) 
	{
		oe_debug_call("vk_debugger");
		
		m_dldy = {};
		m_dldy.init(*m_instance, vkGetInstanceProcAddr);

		auto create_info = createInfo();
		m_debug_messenger = m_instance->createDebugUtilsMessengerEXT(*create_info, nullptr, m_dldy);
		delete create_info;
	}

	Debugger::~Debugger() {
		m_instance->destroyDebugUtilsMessengerEXT(m_debug_messenger, nullptr, m_dldy);
	}

}
#endif