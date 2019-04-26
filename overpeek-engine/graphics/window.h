#pragma once

#include <string>

#define WINDOW_MULTISAMPLE_X2	0x000020
#define WINDOW_MULTISAMPLE_X4	0x000040
#define WINDOW_MULTISAMPLE_X8	0x000080
#define WINDOW_BORDERLESS		0x000001
#define WINDOW_RESIZEABLE		0x000100
#define WINDOW_TRANSPARENT		0x001000
#define WINDOW_FULLSCREEN		0x010000


struct GLFWwindow;
namespace oe {
	class Window {
	private:
		//Variables
		static int mWidth, mHeight;
		static float mAspect;
		const char *mTitle;
		GLFWwindow *mWindow;
		bool m_fullscreen;
		unsigned int m_multisample;
		static double mMouseX, mMouseY;
		bool m_resizeable;
		bool m_transparent;
		bool m_borderless;

		static bool mKeys[];
		static bool mButtons[];
		static bool mSingleKeys[];
		static bool mSingleButtons[];

		static void (*mKeyCallback)(int, int);
		static void (*mButtonCallback)(int, int);
		static void(*m_scroll_callback)(double);
		static void(*m_resize_callback)(int, int);
		static void(*m_charmods_callback)(unsigned int, int);

		//Functions
		bool mInit();
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void framebuffer_size_callback(GLFWwindow *window, int numer, int denom);
		static void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
		
	public:
		//Variables

		
		//Functions
		/*
		width	-	window width
		height	-	window height
		title	-	window title
		mods	-	window modification flags, prefixed with "WINDOW_"		ex.	WINDOW_MULTISAMPLE_X8 | WINDOW_BORDERLESS
		*/
		Window(unsigned int width, unsigned int height, const char *title, int mods);
		~Window();

		static void checkErrors();
		bool close();
		void input();
		void update();
		void clear();
		static void terminate();

		void setButtonCallback(void(*callback)(int, int));
		void setKeyboardCallback(void(*callback)(int, int));
		void setScrollCallback(void(*callback)(double));
		void setResizeCallback(void(*callback)(int, int));
		void setCharmodCallback(void(*callback)(unsigned int, int));

		void setSwapInterval(unsigned int interval);
		void setBackFaceCulling(bool on);

		void setLineWidth(float w);
		void setPointRadius(float w);


		void setClearColor(float r, float g, float b, float a);

		void setIcon(const char *path);

		void showCursor(bool show);
		void setCursorPos(double x, double y);
		inline bool getButton(int button) { return mButtons[button]; }
		inline bool getKey(int key) { return mKeys[key]; }
		inline void getMousePos(double &x, double &y) { x = (mMouseX / mWidth * 2.0 * mAspect) - mAspect; y = mMouseY / mHeight * 2 - 1; }


		inline float getAspect() { return mAspect; }
		inline int getHeight() { return mHeight; }
		inline int getWidth() { return mWidth; }

		std::string getRenderer();
		std::string getVendor();
	};

}