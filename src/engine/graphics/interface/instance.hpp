#pragma once

#include "engine/enum.hpp"



namespace oe::graphics {

	// note: OpenGL supports only one instance
	class Instance {
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