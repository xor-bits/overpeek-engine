#include "window.h"
#include <GL/GLU.h>
#include <Windows.h>

#include "stb_image.h"

#define M_NUM_KEYS		512
#define M_NUM_BUTTONS	128

namespace graphics {


	bool Window::mKeys[M_NUM_KEYS];
	bool Window::mButtons[M_NUM_BUTTONS];
	bool Window::mSingleKeys[M_NUM_KEYS];
	bool Window::mSingleButtons[M_NUM_BUTTONS];

	double Window::mMouseX;
	double Window::mMouseY;

	void(*Window::mKeyCallback)(int, int);
	void(*Window::mButtonCallback)(int, int);
	void(*Window::m_scroll_callback)(double);

	Window::Window(unsigned int width, unsigned int height, std::string title, bool fullscreen) {
		mWidth = width; mHeight = height; mTitle = title; mAspect = width / (float)height;
		m_fullscreen = fullscreen;

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
			std::cerr << "ERROR Initalizing GLFW!" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_RESIZABLE, 0);
		//glfwWindowHint(GLFW_SAMPLES, 16);

		if (m_fullscreen)
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
		else
			mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), NULL, NULL);
		if (!mWindow) {
			std::cerr << "ERROR Creating window!" << std::endl;
			return false;
		}
		glfwMakeContextCurrent(mWindow);

		if (glewInit() != GLEW_OK) {
			std::cerr << "ERROR Initalizing GLEW!" << std::endl;
			return false;
		}
		std::cout << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;


		int width, height, nrChannels;
		GLubyte *data = stbi_load("recourses/icon.png", &width, &height, &nrChannels, 0);
		GLFWimage icon; icon.height = height; icon.width = width; icon.pixels = data;
		glfwSetWindowIcon(mWindow, 1, &icon);

		glfwSetCursorPosCallback(mWindow, cursor_position_callback);
		glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
		glfwSetKeyCallback(mWindow, key_callback);
		glfwSetScrollCallback(mWindow, scroll_callback);
		//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//glEnable(GL_MULTISAMPLE);
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

	void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (m_scroll_callback) m_scroll_callback(yoffset);
	}

	void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (mButtonCallback && action == GLFW_PRESS && !mSingleButtons[button]) { mSingleButtons[button] = true; (*mButtonCallback)(button, action); }
		else if (action == GLFW_RELEASE) mSingleButtons[button] = false;

		if (action == GLFW_PRESS) mButtons[button] = true;
		else if (action == GLFW_RELEASE) mButtons[button] = false;
	}

	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
			std::cout << err << " ";
			switch (err)
			{
			case GL_NO_ERROR:
				std::cout << "No error, lol you shouldn't see this" << std::endl;
				break;
			case GL_INVALID_ENUM:
				std::cout << "Invalid enum!" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cout << "Invalid value!" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cout << "Invalid operation!" << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cout << "Invalid framebuffer operation!" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cout << "Out of memory!" << std::endl;
				break;
			default:
				std::cout << "Unknown error!" << std::endl;
				break;
			}
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