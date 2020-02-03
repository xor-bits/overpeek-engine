#pragma once

#include <string>




namespace oe::graphics {

	class GL {
	public:
		enum class depth_func {
			always, never, less_than, greater_than, equal, less_than_or_equal, greater_than_or_equal
		};
		enum class culling {
			neither, both, front, back
		};
		enum class polygon_mode {
			fill, lines, points
		};
		
		static int __checkGLErrors(const std::string& file, int line);

#ifdef _DEBUG
#define checkGLErrors() __checkGLErrors(__FILE__, __LINE__)
#else
#define checkGLErrors()
#endif

		static void enableBlending();
		static void disableBlending();

		static void enableDepth(depth_func = depth_func::always);
		static void disableDepth();

		/*Also known as VSync*/
		static void setSwapInterval(unsigned int interval = 0);
		static void setCulling(culling = culling::neither);

		static void setLineWidth(float w);
		static void setPointRadius(float w);

		/*0 - fill, 1 - lines, 2 - points*/
		static void setPolygonMode(polygon_mode = polygon_mode::fill);

		static std::string getVersion();
		static std::string getRenderer();
		static std::string getVendor();

	};

}