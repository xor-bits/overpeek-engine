#include "vk_window.hpp"
#include "engine/engine.hpp"

#include <GLFW/glfw3.h>



static void glfw_error_func(int code, const char* desc) {
	spdlog::error("GLFW ({}): {}", code, desc);
}

namespace oe::graphics {

	void VKWindow::glfw() {
		glfwSetErrorCallback(glfw_error_func);
		if (!glfwInit()) oe_error_terminate("Failed to initialize GLFW!");

		// Window hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Window creation
		m_window_handle = glfwCreateWindow(m_window_info.size.x, m_window_info.size.y, m_window_info.title.c_str(), m_window_info.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!m_window_handle) oe_error_terminate("Failed to create window!");

		// Vulkan surface
		auto p_instance = VkInstance(m_instance->m_vk_instance);
		auto p_surface = VkSurfaceKHR();
		if (glfwCreateWindowSurface(p_instance, m_window_handle, nullptr, &p_surface) != VK_SUCCESS) {
			spdlog::error("failed to create window surface");
		}
		m_surface = p_surface;

		Window::postglfw();
	}

	VKWindow::VKWindow(const VKInstance* instance, const WindowInfo& window_config)
		: Window(instance, window_config) 
		, m_debugging(instance->m_instance_info.debug_messages)
		, m_instance(instance)
	{
		spdlog::info("Opening window with Vulkan context");
		if (m_debugging)
			spdlog::warn("Debugger enabled");

		glfw();
	}

	VKWindow::~VKWindow() {
		m_instance->m_vk_instance.destroySurfaceKHR(m_surface);
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}



	void VKWindow::update() {
		// swap framebuffers and wait for semaphores and fences
	}

	void VKWindow::clear(const glm::vec4& color) {
		// clear window framebuffer
	}
	
	void VKWindow::viewport() {
		// no clue how to do this one
	}
	
	void VKWindow::swapInterval(uint8_t frames) {
		// or this one
	}

}