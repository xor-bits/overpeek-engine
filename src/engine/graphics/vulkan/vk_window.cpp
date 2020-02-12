#include "vk_window.hpp"
#include "engine/engine.hpp"

#include "engine/graphics/vulkan/vk_instance.hpp"
#include "engine/graphics/vulkan/vk_physical_device.hpp"
#include "engine/graphics/vulkan/vk_logical_device.hpp"
#include "engine/graphics/vulkan/vk_swapchain.hpp"

#include <GLFW/glfw3.h>
#include <string>
#include <sstream>



namespace oe::graphics {

	void VKWindow::glfw() {
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

		m_physical_device = new PhysicalDevice(m_instance, &m_surface);
		m_logical_device = new LogicalDevice(m_instance, m_physical_device);
		m_swapchain = new SwapChain(this, m_logical_device);
	}

	VKWindow::~VKWindow() {
		delete m_swapchain;
		delete m_logical_device;
		delete m_physical_device;

		m_instance->m_vk_instance.destroySurfaceKHR(m_surface);
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}



	void VKWindow::update() {
		// swap framebuffers
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



	Renderer* VKWindow::createRenderer(const RendererInfo& renderer_info) const
	{
		return nullptr;
	}

	Shader* VKWindow::createShader(const ShaderInfo& shader_info) const
	{
		return nullptr;
	}

	Texture* VKWindow::createTexture(const TextureInfo& texture_info) const
	{
		return nullptr;
	}

	FrameBuffer* VKWindow::createFrameBuffer(const FrameBufferInfo& framebuffer_info) const
	{
		return nullptr;
	}

	void VKWindow::destroyRenderer(Renderer* renderer) const
	{
		delete renderer;
	}

	void VKWindow::destroyShader(Shader* shader) const
	{
		delete shader;
	}

	void VKWindow::destroyTexture(Texture* texture) const
	{
		delete texture;
	}

	void VKWindow::destroyFrameBuffer(FrameBuffer* framebuffer) const
	{
		delete framebuffer;
	}



	std::string VKWindow::getAPI() const
	{
		return "Vulkan";
	}

	std::string VKWindow::getAPIVersion() const
	{
		uint32_t api_version = m_physical_device->m_physical_device_properties.apiVersion;
		std::stringstream ss;
		ss << VK_VERSION_MAJOR(api_version) << "." << VK_VERSION_MINOR(api_version) << "." << VK_VERSION_PATCH(api_version);
		return ss.str();
	}

	std::string VKWindow::getGPU() const
	{
		return m_physical_device->m_physical_device_properties.deviceName;
	}

	std::string VKWindow::getGPUVendor() const
	{
		switch (m_physical_device->m_physical_device_properties.vendorID)
		{
		case 0x1002:
			return "AMD";
		case 0x1010:
			return "ImgTec";
		case 0x10DE:
			return "NVidia";
		case 0x5143:
			return "Qualcomm";
		case 0x8086:
			return "Intel";
		default:
			return "Unknown vendor";
		}
	}

}