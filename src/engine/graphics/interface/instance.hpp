#pragma once

#include "engine/enum.hpp"



namespace oe::graphics {

	class Window; class Renderer; class Shader; class Texture; class FrameBuffer;

	// note: OpenGL supports only one instance
	class Instance {
	public:
		InstanceInfo m_instance_info;

	public:
		Instance(const InstanceInfo& instance_info);
		virtual ~Instance();

		virtual Window* createWindow(const WindowInfo& window_config) const = 0;
		virtual void destroyWindow(Window* window) const = 0;

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

	};

}