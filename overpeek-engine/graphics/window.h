#pragma once

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace graphics {

	class Window {
	private:
		//Variables
		unsigned int mWidth, mHeight;
		float mAspect;
		std::string mTitle;
		GLFWwindow *mWindow;
		bool m_fullscreen;
		static double mMouseX, mMouseY;

		static bool mKeys[];
		static bool mButtons[];
		static bool mSingleKeys[];
		static bool mSingleButtons[];

		static void (*mKeyCallback)(int, int);
		static void (*mButtonCallback)(int, int);
		static void(*m_scroll_callback)(double);

		//Functions
		bool mInit();
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	public:
		//Variables

		
		//Functions
		Window(unsigned int width, unsigned int height, std::string title, bool fullscreen);
		~Window();

		static void checkErrors();
		bool close();
		void input();
		void update();
		void clear();

		void setButtonCallback(void(*callback)(int, int));
		void setKeyboardCallback(void(*callback)(int, int));
		void setScrollCallback(void(*callback)(double));

		inline float getAspect() { return mAspect; }

		inline bool getButton(int button) { return mButtons[button]; }
		inline bool getKey(int key) { return mKeys[key]; }
		inline glm::vec2 getMousePos() { return glm::vec2((mMouseX / mWidth * 2.0 * mAspect) - mAspect, mMouseY / mHeight * 2 - 1); }
	};

}