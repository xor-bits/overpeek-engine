#include "window.h"
#include <GL/GLU.h>
#include <Windows.h>
#include <string>

#include "stb_image.h"
#include "../utility/logger.h"

#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024


///TODO: MAKE WINDOW RESIZEABLE

namespace oe {


	bool Window::mKeys[M_NUM_KEYS];
	bool Window::mButtons[M_NUM_BUTTONS];
	bool Window::mSingleKeys[M_NUM_KEYS];
	bool Window::mSingleButtons[M_NUM_BUTTONS];

	double Window::mMouseX;
	double Window::mMouseY;

	void(*Window::mKeyCallback)(int, int);
	void(*Window::mButtonCallback)(int, int);
	void(*Window::m_scroll_callback)(double);

	Window::Window(unsigned int width, unsigned int height, std::string title, bool fullscreen, unsigned int multisample, bool resizeable)
	{
		mWidth = width; mHeight = height; mTitle = title; mAspect = width / (float)height;
		m_fullscreen = fullscreen; m_multisample = multisample;

		for (int i = 0; i < M_NUM_KEYS; i++) mKeys[i] = false;
		for (int i = 0; i < M_NUM_BUTTONS; i++) mButtons[i] = false;
		for (int i = 0; i < M_NUM_KEYS; i++) mSingleKeys[i] = false;
		for (int i = 0; i < M_NUM_BUTTONS; i++) mSingleButtons[i] = false;

		if (!mInit(resizeable)) {
			glfwTerminate();
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	Window::~Window() {
		glfwTerminate();
	}

	bool Window::mInit(bool resizeable) {
		if (!glfwInit()) {
			oe::Logger::error("Failed to initialize GLFW!");
			return false;
		}
		if (m_multisample != 0) glfwWindowHint(GLFW_SAMPLES, m_multisample);
		glfwWindowHint(GLFW_RESIZABLE, resizeable);

		if (m_fullscreen)
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
		else
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
		if (!mWindow) {
			oe::Logger::error("Failed to create window!");
			return false;
		}
		glfwMakeContextCurrent(mWindow);

		if (glewInit() != GLEW_OK) {
			oe::Logger::error("Failed to initalize GLEW!");
			return false;
		}

		oe::Logger::info("Window created");
		oe::Logger::info("OpenGL Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
		oe::Logger::info("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));


		int width, height, nrChannels;
		GLubyte *data = stbi_load("recourses/icon.png", &width, &height, &nrChannels, 0);
		GLFWimage icon; icon.height = height; icon.width = width; icon.pixels = data;
		glfwSetWindowIcon(mWindow, 1, &icon);

		glfwSetFramebufferSizeCallback(mWindow, glfwSetWindowAspectRatio);
		glfwSetCursorPosCallback(mWindow, cursor_position_callback);
		glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
		glfwSetKeyCallback(mWindow, key_callback);
		glfwSetScrollCallback(mWindow, scroll_callback);
		//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (m_multisample != 0) glEnable(GL_MULTISAMPLE);

		glViewport(0, 0, mWidth, mHeight);
		glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);

		glfwSwapInterval(0);

		return true;
	}

	void Window::setSwapInterval(unsigned int interval) {
		glfwSwapInterval(interval);
	}

	void Window::framebuffer_size_callback(GLFWwindow *window, int numer, int denom) {
		int height, width;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		oe::Logger::info("resized");
	}

	void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (m_scroll_callback) m_scroll_callback(yoffset);
	}

	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button >= M_NUM_BUTTONS || button < 0) return;
		if (mButtonCallback && action == GLFW_PRESS && !mSingleButtons[button]) { mSingleButtons[button] = true; (*mButtonCallback)(button, action); }
		else if (action == GLFW_RELEASE) mSingleButtons[button] = false;

		if (action == GLFW_PRESS) mButtons[button] = true;
		else if (action == GLFW_RELEASE) mButtons[button] = false;
	}

	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key >= M_NUM_KEYS || key < 0) return;

		if (mKeyCallback && action == GLFW_PRESS && !mSingleKeys[key]) { mSingleKeys[key] = true; (*mKeyCallback)(key, action); }
		else if (action == GLFW_RELEASE) mSingleKeys[key] = false;

		if (action == GLFW_PRESS) mKeys[key] = true;
		else if (action == GLFW_RELEASE) mKeys[key] = false;
	}

	void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		mMouseX = xpos; mMouseY = ypos;
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
			oe::Logger::error(std::string("OpenGL ") + std::to_string(err) + std::string(": ") + errorText);
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

}