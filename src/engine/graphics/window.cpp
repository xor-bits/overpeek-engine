#include "window.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "engine/internal_libs.h"
#include "engine/utility/math.h"
#include "engine/graphics/gl.h"

#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



namespace oe::graphics {

	void *Window::p_window;
	int Window::p_width, Window::p_height;

	bool Window::p_keys[M_NUM_KEYS];
	bool Window::p_buttons[M_NUM_BUTTONS];
	
	bool Window::p_debugmode = false;

	glm::vec2 Window::p_mouse;

	void(*Window::mKeyCallback)(int, int);
	void(*Window::mButtonCallback)(int, int);
	void(*Window::m_scroll_callback)(double);
	void(*Window::m_resize_callback)(int, int);
	void(*Window::m_charmods_callback)(unsigned int, int);

	int Window::init(unsigned int width, unsigned int height, std::string title, int mods)
	{
		p_width = width; p_height = height;

		bool resizeable = false;
		bool transparent = false;
		bool borderless = false;
		bool fullscreen = false; 
		int multisample = 0;

		if (mods & WINDOW_MULTISAMPLE_X2) multisample = 2;
		if (mods & WINDOW_MULTISAMPLE_X4) multisample = 4;
		if (mods & WINDOW_MULTISAMPLE_X8) multisample = 8;
		if (mods & WINDOW_TRANSPARENT) transparent = true;
		if (mods & WINDOW_BORDERLESS) borderless = true;
		if (mods & WINDOW_RESIZEABLE) resizeable = true;
		if (mods & WINDOW_FULLSCREEN) fullscreen = true;
		if (mods & WINDOW_GL_DEBUG) p_debugmode = true;

		for (int i = 0; i < M_NUM_KEYS; i++) p_keys[i] = false;
		for (int i = 0; i < M_NUM_BUTTONS; i++) p_buttons[i] = false;


		// glfw
		//--------------
		if (!glfwInit()) {
			spdlog::error("Failed to initialize GLFW!");
			return -1;
		}

		if (multisample != 0) glfwWindowHint(GLFW_SAMPLES, multisample);
		glfwWindowHint(GLFW_RESIZABLE, resizeable);
#ifndef EMSCRIPTEN
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, transparent);
#endif
		glfwWindowHint(GLFW_DECORATED, !borderless);

		if (fullscreen)
			p_window = glfwCreateWindow(p_width, p_height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		else
			p_window = glfwCreateWindow(p_width, p_height, title.c_str(), NULL, NULL);
		
		if (!p_window) {
			spdlog::error("Failed to create window!");
			return -2;
		}

#ifndef EMSCRIPTEN
		//Center the window
		int monitor_xpos, monitor_ypos, monitor_width, monitor_height;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);
		glfwSetWindowPos((GLFWwindow*)p_window, monitor_width / 2.0f - p_width / 2.0f, monitor_height / 2.0f - p_height / 2.0f);
#endif

		// opengl/glew
		//--------
		glfwMakeContextCurrent((GLFWwindow*)p_window);

		if (glewInit() != GLEW_OK) {
			spdlog::error("Failed to initalize GLEW");
			return -3;
		}

		spdlog::info("Window created");
		spdlog::info("OpenGL Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
		spdlog::info("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));

		glfwSetCharModsCallback((GLFWwindow*)p_window, [](GLFWwindow* window, unsigned int codepoint, int mods) { charmods_callback(codepoint, mods); });
		glfwSetFramebufferSizeCallback((GLFWwindow*)p_window, [](GLFWwindow* window, int width, int height) { framebuffer_size_callback(width, height); });
		glfwSetCursorPosCallback((GLFWwindow*)p_window, [](GLFWwindow* window, double x, double y) { cursor_position_callback(x, y); });
		glfwSetMouseButtonCallback((GLFWwindow*)p_window, [](GLFWwindow* window, int button, int action, int mods) { mouse_button_callback(button, action, mods); });
		glfwSetKeyCallback((GLFWwindow*)p_window, [](GLFWwindow* window, int key, int scancode, int action, int mode) { key_callback(key, scancode, action, mode); });
		glfwSetScrollCallback((GLFWwindow*)p_window, [](GLFWwindow* window, double xoffset, double yoffset) { scroll_callback(xoffset, yoffset); });

		//glfwSetInputMode(mWindow, OE_CURSOR, OE_CURSOR_DISABLED);

		if (multisample != 0) glEnable(GL_MULTISAMPLE);

		glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
		viewport();
	}

	void Window::viewport() {
		glViewport(0, 0, p_width, p_height);
	}

	void Window::showCursor(bool show) {
		if (!show) glfwSetInputMode((GLFWwindow*)p_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode((GLFWwindow*)p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::setIcon(const oe::utils::image_data& image) {
		GLFWimage glfwicon; 
		glfwicon.height = image.height; glfwicon.width = image.width; glfwicon.pixels = image.data;
		glfwSetWindowIcon((GLFWwindow*)p_window, 1, &glfwicon);
	}

	void Window::setClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Window::framebuffer_size_callback(int width, int height) {
		p_width = width; p_height = height;
		if (p_height <= 0) p_height = 1;
		glViewport(0, 0, p_width, p_height);

		if (m_resize_callback) m_resize_callback(p_width, p_height);
	}

	void Window::scroll_callback(double xoffset, double yoffset)
	{
		if (m_scroll_callback) m_scroll_callback(yoffset);
	}

	void Window::mouse_button_callback(int button, int action, int mods) {
		if (button >= M_NUM_BUTTONS || button < 0) return;

		if (action == GLFW_PRESS) p_buttons[button] = true;
		else if (action == GLFW_RELEASE) p_buttons[button] = false;

		if (mButtonCallback) (*mButtonCallback)(button, action);
	}

	void Window::key_callback(int key, int scancode, int action, int mods) {
		if (key >= M_NUM_KEYS || key < 0) return;

		if (action == GLFW_PRESS) p_keys[key] = true;
		else if (action == GLFW_RELEASE) p_keys[key] = false;

		if (mKeyCallback) (*mKeyCallback)(key, action);
	}

	void Window::setCursorPos(double x, double y) {
		glfwSetCursorPos((GLFWwindow*)p_window, x, y);
		p_mouse = glm::vec2(x, y);
	}

	void Window::cursor_position_callback(double xpos, double ypos)
	{
		xpos = oe::utils::map((double)xpos, (double)0.0, (double)p_width, (double)-getAspect(), (double)getAspect());
		ypos = oe::utils::map((double)ypos, (double)0.0, (double)p_height, (double)-1.0, (double)1.0);
		p_mouse = glm::vec2(xpos, ypos);
	}

	void Window::charmods_callback(unsigned int codepoint, int mods) {
		if (m_charmods_callback) m_charmods_callback(codepoint, mods);
	}

	bool Window::shouldClose() {
		return glfwWindowShouldClose((GLFWwindow*)p_window) == 1;
	}

	void Window::input() {
		glfwPollEvents();
	}

	void Window::update() {
		if (p_debugmode) GL::checkGLErrors();
		glfwSwapBuffers((GLFWwindow*)p_window);
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

	void Window::close() {
		glfwTerminate();
	}

}