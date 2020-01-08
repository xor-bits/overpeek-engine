#pragma once

#include <string>




namespace oe::graphics {

	class GL {
	public:
		static int __checkGLErrors(const std::string& file, int line);

#ifdef _DEBUG
#define checkGLErrors() __checkGLErrors(__FILE__, __LINE__)
#else
#define checkGLErrors()
#endif

		static void enableBlending();
		static void disableBlending();

		static void enableDepth();
		static void disableDepth();

		/*Also known as VSync*/
		static void setSwapInterval(unsigned int interval);
		static void setBackFaceCulling(bool on);

		static void setLineWidth(float w);
		static void setPointRadius(float w);

		/*0 - fill, 1 - lines, 2 - points*/
		static void setPolygonMode(int n);

		static std::string getVersion();
		static std::string getRenderer();
		static std::string getVendor();

	};

}