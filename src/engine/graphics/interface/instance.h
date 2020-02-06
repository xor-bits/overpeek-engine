#pragma once

#include "engine/enum.h"



namespace oe::graphics {

	class Window; class Renderer; class Shader; class Texture; class FrameBuffer;

	// note: OpenGL supports only one instance
	class Instance {
	public:
		InstanceInfo m_instance_info;

	public:
		Instance(const InstanceInfo& instance_info);
		~Instance();

		virtual Window* createWindow(const WindowInfo& window_config) const = 0;
		virtual Renderer* createRenderer(const RendererInfo& renderer_info) const = 0;
		virtual Shader* createShader(const ShaderInfo& shader_info) const = 0;
		virtual Texture* createTexture(const TextureInfo& texture_info) const = 0;
		virtual FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info) const = 0;
		
		virtual void destroyWindow(Window* window) const = 0;
		virtual void destroyRenderer(Renderer* renderer) const = 0;
		virtual void destroyShader(Shader* shader) const = 0;
		virtual void destroyTexture(Texture* texture) const = 0;
		virtual void destroyFrameBuffer(FrameBuffer* framebuffer) const = 0;

	public:
		// set blending mode
		virtual void blending(oe::modes mode = oe::modes::enable) const = 0;
		
		// set depth function or disable it
		virtual void depth(depth_functions func = depth_functions::always) const = 0;

		// also known as vertical sync
		virtual void swapInterval(unsigned int interval = 0) const = 0;
		
		// stop drawing any side of primitives
		virtual void culling(culling_modes c = culling_modes::neither) const = 0;

		// width of all lines in pixels
		virtual void lineWidth(float w = 1.0f) const = 0;

		// radius of all points in pixels
		virtual void pointRadius(float w = 1.0f) const = 0;

		// primitive draw mode
		virtual void polygonMode(polygon_mode p = polygon_mode::fill) const = 0;



		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;

	};

}