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
		void vulkanPhysicalDevice();

		// Inherited via Instance
		virtual Window* createWindow(const WindowInfo& window_config) const override;
		virtual Renderer* createRenderer(const RendererInfo& renderer_info) const override;
		virtual Shader* createShader(const ShaderInfo& shader_info) const override;
		virtual Texture* createTexture(const TextureInfo& texture_info) const override;
		virtual FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const override;

		// Inherited via Instance
		virtual void destroyWindow(Window* window) const override;
		virtual void destroyRenderer(Renderer* renderer) const override;
		virtual void destroyShader(Shader* shader) const override;
		virtual void destroyTexture(Texture* texture) const override;
		virtual void destroyFrameBuffer(FrameBuffer* framebuffer) const override;

	public:
		// Inherited via Instance
		virtual void blending(oe::modes mode = oe::modes::enable) const override;
		virtual void depth(depth_functions func = depth_functions::always) const override;
		virtual void swapInterval(unsigned int interval = 0) const override;
		virtual void culling(culling_modes c = culling_modes::neither) const override;
		virtual void lineWidth(float w = 1.0f) const override;
		virtual void pointRadius(float w = 1.0f) const override;
		virtual void polygonMode(polygon_mode p = polygon_mode::fill) const override;

		// Inherited via Instance
		virtual std::string getAPI() const override;
		virtual std::string getAPIVersion() const override;
		virtual std::string getGPU() const override;
		virtual std::string getGPUVendor() const override;

	};

}