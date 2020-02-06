#pragma once

#include "instance.hpp"



static void glfw_error_func(int code, const char* desc) {
	errorLog("GLFW ({}): {}", code, desc);
}

namespace oe::vulkan {

	std::string cpu_name;
	void init() {
		char* brand = new char[49];
		cpu_features::FillX86BrandString(brand);
		cpu_name = brand;
		delete brand;

		// glfw error callback function
		glfwSetErrorCallback(glfw_error_func);

		// initialize glfw
		if (glfwInit() != GLFW_TRUE) {
			// glfw init error
			errorLog("GLFW init failed");
		}
	}

	class Window {
	public:
		vk::SurfaceKHR m_surface;
		GLFWwindow* m_window;

		const Instance* m_instance;

	public:
		Window(const Instance* instance) : m_instance(instance) {
			// set up glfw window hints
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			// open glfw window
			m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
			if (!m_window) {
				// window open error
				errorLog("Window creation failed");
			}

			// Vulkan surface
			auto p_instance = VkInstance(m_instance->m_instance);
			auto p_surface = VkSurfaceKHR();
			if (glfwCreateWindowSurface(p_instance, m_window, nullptr, &p_surface) != VK_SUCCESS) {
				errorLog("failed to create window surface");
			}
			m_surface = p_surface;
		}

		~Window() {
			m_instance->m_instance.destroySurfaceKHR(m_surface);
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		void update() {
			glfwPollEvents();
		}
	
	};

}