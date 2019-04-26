#include "window.h"

#include <Windows.h>
#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "../utility/logger.h"

#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024


///TODO: MAKE WINDOW RESIZEABLE

namespace oe {

	float Window::mAspect;
	int Window::mWidth, Window::mHeight;

	bool Window::mKeys[M_NUM_KEYS];
	bool Window::mButtons[M_NUM_BUTTONS];
	bool Window::mSingleKeys[M_NUM_KEYS];
	bool Window::mSingleButtons[M_NUM_BUTTONS];

	double Window::mMouseX;
	double Window::mMouseY;

	void(*Window::mKeyCallback)(int, int);
	void(*Window::mButtonCallback)(int, int);
	void(*Window::m_scroll_callback)(double);
	void(*Window::m_resize_callback)(int, int);
	void(*Window::m_charmods_callback)(unsigned int, int);

	Window::Window(unsigned int width, unsigned int height, const char *title, int mods)
	{
		m_resizeable = false;
		m_transparent = false;
		m_borderless = false;

		m_fullscreen = false; 
		m_multisample = 0;
		if (mods & WINDOW_MULTISAMPLE_X2) {
			//oe::Logger::out("WINDOW MULTISAMPLE X2"); 
			m_multisample = 2;
		}
		if (mods & WINDOW_MULTISAMPLE_X4) {
			//oe::Logger::out("WINDOW MULTISAMPLE X4");
			m_multisample = 4;
		}
		if (mods & WINDOW_MULTISAMPLE_X8) {
			//oe::Logger::out("WINDOW MULTISAMPLE X8");
			m_multisample = 8;
		}
		if (mods & WINDOW_TRANSPARENT) {
			//oe::Logger::out("WINDOW TRANSPARENT");
			m_transparent = true;
		}
		if (mods & WINDOW_BORDERLESS) {
			//oe::Logger::out("WINDOW BORDERLESS");
			m_borderless = true;
		}
		if (mods & WINDOW_RESIZEABLE) {
			//oe::Logger::out("WINDOW RESIZEABLE");
			m_resizeable = true;
		}
		if (mods & WINDOW_FULLSCREEN) {
			//oe::Logger::out("WINDOW FULLSCREEN");
			m_fullscreen = true;
		}


		mWidth = width; mHeight = height; mTitle = title; mAspect = width / (float)height;

		for (int i = 0; i < M_NUM_KEYS; i++) mKeys[i] = false;
		for (int i = 0; i < M_NUM_BUTTONS; i++) mButtons[i] = false;
		for (int i = 0; i < M_NUM_KEYS; i++) mSingleKeys[i] = false;
		for (int i = 0; i < M_NUM_BUTTONS; i++) mSingleButtons[i] = false;

		if (!mInit()) {
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	Window::~Window() {
		glfwTerminate();
	}

	bool Window::mInit() {
		if (!glfwInit()) {
			oe::Logger::out("Failed to initialize GLFW!", oe::error);
			return false;
		}
		
		//Vulkan
		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//

		if (m_multisample != 0) glfwWindowHint(GLFW_SAMPLES, m_multisample);
		glfwWindowHint(GLFW_RESIZABLE, m_resizeable);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_transparent);
		glfwWindowHint(GLFW_DECORATED, !m_borderless);

		if (m_fullscreen)
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, glfwGetPrimaryMonitor(), NULL);
		else
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, NULL, NULL);
		if (!mWindow) {
			oe::Logger::out("Failed to create window!", oe::error);
			return false;
		}

		//Center the window
		int xpos, ypos, width, height;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
		glfwSetWindowPos(mWindow, width / 2.0f - mWidth / 2.0f, height / 2.0f - mHeight / 2.0f);

		//glfwSetWindowOpacity(mWindow, 0.5f);
		//------
		//Vulkan
		//------


		////Application info
		//VkApplicationInfo appInfo = {};
		//appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		//appInfo.pApplicationName = "Unnamed Application";
		//appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		//appInfo.pEngineName = "Overpeek Engine";
		//appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		//appInfo.apiVersion = VK_API_VERSION_1_0;
		//
		////Instance create info
		//VkInstanceCreateInfo createInfo = {};
		//createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		//createInfo.pApplicationInfo = &appInfo;
		//
		////GLFW extensions
		//uint32_t glfwExtensionCount = 0;
		//const char** glfwExtensions;
		//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		//createInfo.enabledExtensionCount = glfwExtensionCount;
		//createInfo.ppEnabledExtensionNames = glfwExtensions;
		//createInfo.enabledLayerCount = 0;
		//
		////Create instance
		//VkInstance instance;
		//if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to create instance!");
		//}
		//
		//while(true) {}
		//vkDestroyInstance(instance, nullptr);

		//------
		//OpenGL
		//------
		glfwMakeContextCurrent(mWindow);

		if (glewInit() != GLEW_OK) {
			oe::Logger::out("Failed to initalize GLEW!", oe::error);
			return false;
		}

		oe::Logger::out("Window created", oe::info);
		oe::Logger::out("OpenGL Renderer: ", (char*)glGetString(GL_RENDERER), oe::info);
		oe::Logger::out("OpenGL Version: ", (char*)glGetString(GL_VERSION), oe::info);

		glfwSetCharModsCallback(mWindow, charmods_callback);
		glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
		glfwSetCursorPosCallback(mWindow, cursor_position_callback);
		glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
		glfwSetKeyCallback(mWindow, key_callback);
		glfwSetScrollCallback(mWindow, scroll_callback);
		
		
		//glfwSetInputMode(mWindow, OE_CURSOR, OE_CURSOR_DISABLED);

		if (m_multisample != 0) glEnable(GL_MULTISAMPLE);

		glViewport(0, 0, mWidth, mHeight);
		glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL); 
		//glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glfwSwapInterval(0);

		return true;
	}

	void Window::showCursor(bool show) {
		if (!show) glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::setLineWidth(float w) {
		glLineWidth(w);
	}

	void Window::setPointRadius(float w) {
		glPointSize(w);
	}

	void Window::setBackFaceCulling(bool on) {
		if (on) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}

	void Window::setIcon(const char *path) {
		int width, height, nrChannels;
		GLubyte *data = stbi_load(path, &width, &height, &nrChannels, 0);
		GLFWimage icon; icon.height = height; icon.width = width; icon.pixels = data;
		glfwSetWindowIcon(mWindow, 1, &icon);
	}

	void Window::setClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Window::setSwapInterval(unsigned int interval) {
		glfwSwapInterval(interval);
	}

	void Window::framebuffer_size_callback(GLFWwindow *window, int numer, int denom) {
		glfwGetFramebufferSize(window, &mWidth, &mHeight);

		if (mHeight <= 0) {
			mHeight = 1;
			glfwSetWindowSize(window, mWidth, mHeight);
		}

		mAspect = mWidth / (float)mHeight;

		glViewport(0, 0, mWidth, mHeight);

		if (m_resize_callback) m_resize_callback(mWidth, mHeight);
	}

	void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (m_scroll_callback) m_scroll_callback(yoffset);
	}

	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button >= M_NUM_BUTTONS || button < 0) return;
		if (mButtonCallback && action == GLFW_PRESS && !mSingleButtons[button]) { mSingleButtons[button] = true; }
		else if (action == GLFW_RELEASE) mSingleButtons[button] = false;

		if (action == GLFW_PRESS) mButtons[button] = true;
		else if (action == GLFW_RELEASE) mButtons[button] = false;

		if (mButtonCallback) (*mButtonCallback)(button, action);
	}

	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key >= M_NUM_KEYS || key < 0) return;

		if (mKeyCallback && action == GLFW_PRESS && !mSingleKeys[key]) { mSingleKeys[key] = true; }
		else if (action == GLFW_RELEASE) mSingleKeys[key] = false;

		if (action == GLFW_PRESS) mKeys[key] = true;
		else if (action == GLFW_RELEASE) mKeys[key] = false;

		if (mKeyCallback) (*mKeyCallback)(key, action);
	}

	void Window::setCursorPos(double x, double y) {
		glfwSetCursorPos(mWindow, x, y);
		mMouseX = x; mMouseY = y;
	}

	void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		mMouseX = xpos; mMouseY = ypos;
	}

	void Window::charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods) {
		if (m_charmods_callback) m_charmods_callback(codepoint, mods);
	}

	void Window::checkErrors() {
		GLenum err = glGetError();
		if (err != 0) {
			std::string errorText;
			switch (err)
			{
			case GL_NO_ERROR:
				errorText = "If you see this, your processor is propably broken or something?";
				break;
			case GL_INVALID_ENUM:
				errorText = "Invalid enum!";
				break;
			case GL_INVALID_VALUE:
				errorText = "Invalid value!";
				break;
			case GL_INVALID_OPERATION:
				errorText = "Invalid operation!";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorText = "Invalid framebuffer operation!";
				break;
			case GL_OUT_OF_MEMORY:
				errorText = "Out of memory!";
				break;
			default:
				errorText = "Unknown error!";
				break;
			}
			oe::Logger::out(("OpenGL " + std::to_string(err) + std::string(": ") + errorText).c_str(), oe::error);
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	bool Window::close() {
		return glfwWindowShouldClose(mWindow) == 1;
	}

	void Window::input() {
		glfwPollEvents();
	}

	void Window::update() {
		checkErrors();
		glfwSwapBuffers(mWindow);
	}

	void Window::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::setButtonCallback(void(*callback)(int, int)) {
		mButtonCallback = callback;
	}

	void Window::setKeyboardCallback(void (*callback)(int, int)) {
		mKeyCallback = callback;
	}

	void Window::setScrollCallback(void(*callback)(double)) {
		m_scroll_callback = callback;
	}

	void Window::setResizeCallback(void(*callback)(int, int)) {
		m_resize_callback = callback;
	}

	void Window::setCharmodCallback(void(*callback)(unsigned int, int)) {
		m_charmods_callback = callback;
	}

	void Window::terminate() {
		glfwTerminate();
		system("pause");
		exit(1);
	}

	std::string Window::getRenderer() {
		return std::string((char*)glGetString(GL_RENDERER));
	}

	std::string Window::getVendor() {
		return std::string((char*)glGetString(GL_VENDOR));
	}

}