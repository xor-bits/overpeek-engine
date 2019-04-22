#pragma once

#include <string>

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
		bool mInit(bool resizeable);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void framebuffer_size_callback(GLFWwindow *window, int numer, int denom);
		static void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
		
	public:
		//Variables

		
		//Functions
		Window(unsigned int width, unsigned int height, const char *title, bool fullscreen, unsigned int multisample, bool resizeable);
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

		void showCursor(bool show);

		void setClearColor(float r, float g, float b, float a);

		void setIcon(const char *path);

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