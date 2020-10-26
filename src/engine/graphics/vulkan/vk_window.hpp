#pragma once
#ifdef BUILD_VULKAN

#include "engine/graphics/interface/window.hpp"
#include "engine/internal_libs.hpp"

#include <vulkan/vulkan.hpp>
#include <string>


namespace oe::graphics {

	class VKSwapChain;
	class VKSyncObjects;
	class VKCommandPool;
	class VKLogicalDevice;
	class VKPhysicalDevice;
	class VKInstance;
	class VKWindow : public Window {
	public:
		const VKInstance* m_instance;
		vk::SurfaceKHR m_surface;
		bool m_debugging;

		VKPhysicalDevice* m_physical_device;
		VKLogicalDevice* m_logical_device;
		VKSwapChain* m_swapchain;
		VKCommandPool* m_command_pool;
		VKSyncObjects* m_sync_objects;

	public:
		VKWindow(const VKInstance* instance, const WindowInfo& window_config);
		~VKWindow();

		void glfw();

		// Inherited via Window
		void update() override;
		void clear(const glm::vec4& color = oe::colors::clear_color) override;
		void viewport() override;
		void swapInterval(uint8_t frames) override;

		Renderer* createRenderer(const RendererInfo& renderer_info) const override;
		IShader* createShader(const ShaderInfo& shader_info) const override;
		ITexture* createTexture(const TextureInfo& texture_info) const override;
		IFrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const override;
		
		void destroyRenderer(Renderer* renderer) const override;
		void destroyShader(IShader* shader) const override;
		void destroyTexture(ITexture* texture) const override;
		void destroyFrameBuffer(IFrameBuffer* framebuffer) const override;

		// Inherited via Instance
		std::string getAPI() const override;
		std::string getAPIVersion() const override;
		std::string getGPU() const override;
		std::string getGPUVendor() const override;
	};

}
#endif