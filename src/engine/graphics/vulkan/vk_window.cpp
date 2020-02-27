#include "vk_window.hpp"
#include "engine/engine.hpp"

#include "vk_instance.hpp"
#include "vk_shader.hpp"
#include "vk_physical_device.hpp"
#include "vk_logical_device.hpp"
#include "vk_swapchain.hpp"
#include "vk_sync_objects.hpp"
#include "buffers/vk_command_pool.hpp"

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
		if (m_debugging) {
			spdlog::warn("Debugger enabled");
			spdlog::info("Opening window with Vulkan context");
		}

		glfw();

		m_physical_device = new VKPhysicalDevice(m_instance, &m_surface);
		m_logical_device = new VKLogicalDevice(m_instance, m_physical_device);
		m_swapchain = new VKSwapChain(this, m_logical_device);
		m_command_pool = new VKCommandPool(m_physical_device, m_logical_device);
		m_command_pool->createCommandBuffersSwapChain(m_swapchain);
		m_sync_objects = new VKSyncObjects(m_swapchain, m_logical_device);
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
		m_command_pool->endRecording();

		uint32_t imageIndex;
		imageIndex = m_logical_device->m_logical_device.acquireNextImageKHR(m_swapchain->m_swap_chain, UINT64_MAX, m_sync_objects->m_image_available_semaphores[m_sync_objects->currentFrame], nullptr).value;

		m_logical_device->m_logical_device.waitForFences(m_sync_objects->m_in_flight_fences, true, UINT64_MAX);

		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (m_sync_objects->m_images_in_flight[imageIndex]) {
			m_sync_objects->m_logical_device->m_logical_device.waitForFences(m_sync_objects->m_images_in_flight[imageIndex], true, UINT64_MAX);
		}
		// Mark the image as now being in use by this frame
		m_sync_objects->m_images_in_flight[imageIndex] = m_sync_objects->m_images_in_flight[m_sync_objects->currentFrame];

		// submit info
		vk::SubmitInfo submitInfo = {};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_command_pool->m_command_buffers.at(imageIndex)->m_command_buffer;

		// waiting for semaphores
		vk::Semaphore waitSemaphores[] = { m_sync_objects->m_image_available_semaphores[m_sync_objects->currentFrame] };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		// semaphores
		vk::Semaphore signalSemaphores[] = { m_sync_objects->m_render_finished_semaphores[m_sync_objects->currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		m_logical_device->m_logical_device.resetFences(m_sync_objects->m_in_flight_fences[m_sync_objects->currentFrame]);

		// submitting queue
		m_logical_device->m_graphics_queue.submit(submitInfo, m_sync_objects->m_in_flight_fences[m_sync_objects->currentFrame]);

		// presentation
		vk::PresentInfoKHR presentInfo = {};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		vk::SwapchainKHR swapChains[] = { m_swapchain->m_swap_chain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		// present
		m_sync_objects->m_logical_device->m_present_queue.presentKHR(presentInfo);

		m_sync_objects->currentFrame = (m_sync_objects->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		// input
		glfwPollEvents();
	}

	void VKWindow::clear(const glm::vec4& color) {
		m_logical_device->m_logical_device.waitIdle();
		m_command_pool->beginRecording(color);
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
		return new VKShader(this, shader_info);
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
		// delete static_cast<VKRenderer*>(renderer);
	}

	void VKWindow::destroyShader(Shader* shader) const
	{
		delete static_cast<VKShader*>(shader);
	}

	void VKWindow::destroyTexture(Texture* texture) const
	{
		// delete static_cast<VKTexture*>(texture);
	}

	void VKWindow::destroyFrameBuffer(FrameBuffer* framebuffer) const
	{
		// delete static_cast<VKFrameBuffer*>(framebuffer);
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