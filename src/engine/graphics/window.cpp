#include "window.h"

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/internal_libs.h"
#include "engine/utility/math.h"
#include "engine/graphics/gl.h"

#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



namespace oe::graphics {

	Window::WindowConfig m_window;
	GLFWwindow* m_window_handle = nullptr;

	bool m_keys[M_NUM_KEYS];
	bool m_buttons[M_NUM_BUTTONS];

	void(*m_key_callback)(int, int) = nullptr;
	void(*m_button_callback)(int, int) = nullptr;
	void(*m_scroll_callback)(double) = nullptr;
	void(*m_resize_callback)(int, int) = nullptr;
	void(*m_charmods_callback)(unsigned int, int) = nullptr;
	void(*m_cursor_position_callback)(const glm::vec2&, const glm::vec2&) = nullptr;

	glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
	glm::vec2 m_cursor_window = { 0, 0 };
	float m_aspect_ratio = 0;

	

	// Callbacks
	void framebuffer_size_callback(int width, int height);
	void scroll_callback(double xoffset, double yoffset);
	void mouse_button_callback(int button, int action, int mods);
	void key_callback(int key, int scancode, int action, int mods);
	void cursor_position_callback(double xpos, double ypos);
	void charmods_callback(unsigned int codepoint, int mods);



	// others

	void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::string log_text = "OpenGL Debug Message";
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         spdlog::error(log_text); break;
			case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::critical(log_text); break;
			case GL_DEBUG_SEVERITY_LOW:          spdlog::warn(log_text); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info(log_text); break;
		}

		std::string log_source;
		switch (source) {
			case GL_DEBUG_SOURCE_API:             log_source = "API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   log_source = "Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: log_source = "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     log_source = "Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     log_source = "Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           log_source = "Other"; break;
		}

		std::string log_type;
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:               log_type = "Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: log_type = "Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  log_type = "Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         log_type = "Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         log_type = "Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              log_type = "Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          log_type = "Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           log_type = "Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               log_type = "Other"; break;
		}

		std::string log_severity;
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         log_severity = "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       log_severity = "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          log_severity = "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: log_severity = "Severity: notification"; break;
		}

		std::cout << fmt::format("Message       :   ({}): {}", id, message) << std::endl;
		std::cout << fmt::format("Source        :   {}", log_source) << std::endl;
		std::cout << fmt::format("Description   :   {}", log_type) << std::endl;
		std::cout << fmt::format("Line          :   {}", log_severity) << std::endl;
		std::cout << std::endl;
		oe::terminate();
	}

	void glfwError(int id, const char* description) {
		oe_error_terminate("GLFW ({}): {}", id, description);
	}

	void glfw() {
		spdlog::info("Initializing GLFW");
		glfwSetErrorCallback(glfwError);
		if (!glfwInit()) oe_error_terminate("Failed to initialize GLFW!");

		// Window hints
		glfwWindowHint(GLFW_SAMPLES, m_window.multisamples);
		glfwWindowHint(GLFW_RESIZABLE, m_window.resizeable);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_window.transparent);
		glfwWindowHint(GLFW_DECORATED, !m_window.borderless);
		if (m_window.opengl_debugmode) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		// Window creation
		m_window_handle = glfwCreateWindow(m_window.size.x, m_window.size.y, m_window.title.c_str(), m_window.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!m_window_handle) oe_error_terminate("Failed to create window!");

		//Center the window
		int monitor_xpos, monitor_ypos, monitor_width, monitor_height;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);
		m_window.position = { (monitor_width - m_window.size.x) * 0.5f, (monitor_height - m_window.size.y) * 0.5f };
		glfwSetWindowPos(m_window_handle, m_window.position.x, m_window.position.y);

		// set context for opengl
		glfwMakeContextCurrent(m_window_handle);
	}

	void glad() {
		// Init glad
		spdlog::info("Initializing glad");
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) oe_error_terminate("Failed to init glad");

		// GL config
		if (m_window.multisamples) glEnable(GL_MULTISAMPLE);
		if (m_window.opengl_debugmode) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		Window::viewport();
	}

	void postglfw() {
		glfwSetCharModsCallback         (m_window_handle, [](GLFWwindow* window, unsigned int codepoint, int mods)               { charmods_callback(codepoint, mods);			});
		glfwSetFramebufferSizeCallback  (m_window_handle, [](GLFWwindow* window, int width, int height)                          { framebuffer_size_callback(width, height);		});
		glfwSetCursorPosCallback        (m_window_handle, [](GLFWwindow* window, double x, double y)                             { cursor_position_callback(x, y);				});
		glfwSetMouseButtonCallback      (m_window_handle, [](GLFWwindow* window, int button, int action, int mods)               { mouse_button_callback(button, action, mods);	});
		glfwSetKeyCallback              (m_window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mode)    { key_callback(key, scancode, action, mode);	});
		glfwSetScrollCallback           (m_window_handle, [](GLFWwindow* window, double xoffset, double yoffset)                 { scroll_callback(xoffset, yoffset);			});
	}

	void Window::init(WindowConfig& window_config) {
		m_window = window_config;

		// glfw/opengl/glad
		glfw();
		glad();
		postglfw();

		spdlog::info("OpenGL Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
		spdlog::info("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));

		framebuffer_size_callback(m_window.size.x, m_window.size.y);
	}



	bool Window::windowShouldClose() {
		return glfwWindowShouldClose(m_window_handle);
	}



	void Window::pollEvents() {
		glfwPollEvents();
	}

	void Window::updateWindow() {
		glfwSwapBuffers(m_window_handle);
	}

	void Window::clearWindow(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::closeWindow() {
		glfwTerminate();
	}



	void Window::viewport() {
		glViewport(0, 0, m_window.size.x, m_window.size.y);
	}

	void Window::setIcon(const oe::utils::image_data& image) {
		GLFWimage glfwicon; 
		glfwicon.height = image.height; glfwicon.width = image.width; glfwicon.pixels = image.data;
		glfwSetWindowIcon(m_window_handle, 1, &glfwicon);
	}

	void Window::showCursor(bool show) {
		if (!show) glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	

	void Window::setButtonCallback(void(*callback)(int, int)) { m_button_callback = callback; }
	void Window::setKeyboardCallback(void (*callback)(int, int)) { m_key_callback = callback; }
	void Window::setScrollCallback(void(*callback)(double)) { m_scroll_callback = callback; }
	void Window::setResizeCallback(void(*callback)(int, int)) { m_resize_callback = callback; }
	void Window::setCharmodCallback(void(*callback)(unsigned int, int)) { m_charmods_callback = callback; }
	void Window::setCursorPositionCallback(void(*callback)(const glm::vec2&, const glm::vec2&)) { m_cursor_position_callback = callback; }



	// callbacks
	void framebuffer_size_callback(int width, int height) {
		if (height < 1) height = 1;

		m_window.size.x = width; m_window.size.y = height;
		m_aspect_ratio = (float)width / (float)height;
		glViewport(0, 0, m_window.size.x, m_window.size.y);

		if (m_resize_callback) m_resize_callback(m_window.size.x, m_window.size.y);
	}

	void scroll_callback(double xoffset, double yoffset) {
		if (m_scroll_callback) m_scroll_callback(yoffset);
	}

	void mouse_button_callback(int button, int action, int mods) {
		if (button >= M_NUM_BUTTONS || button < 0) return;

		if (action == GLFW_PRESS) m_buttons[button] = true;
		else if (action == GLFW_RELEASE) m_buttons[button] = false;

		if (m_button_callback) (*m_button_callback)(button, action);
	}

	void key_callback(int key, int scancode, int action, int mods) {
		if (key >= M_NUM_KEYS || key < 0) return;

		if (action == GLFW_PRESS) m_keys[key] = true;
		else if (action == GLFW_RELEASE) m_keys[key] = false;

		if (m_key_callback) (*m_key_callback)(key, action);
	}

	void cursor_position_callback(double xpos, double ypos) {
		m_cursor_window = { xpos, ypos };

		xpos = oe::utils::map((double)xpos, (double)0.0, (double)m_window.size.x, (double)-m_aspect_ratio, (double)m_aspect_ratio);
		ypos = oe::utils::map((double)ypos, (double)0.0, (double)m_window.size.y, (double)-1.0, (double)1.0);
		m_cursor_transformed = { xpos, ypos };

		if (m_cursor_position_callback) m_cursor_position_callback(m_cursor_window, m_cursor_transformed);
	}

	void charmods_callback(unsigned int codepoint, int mods) {
		if (m_charmods_callback) m_charmods_callback(codepoint, mods);
	}



	void makeFullscreen() {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		// const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
		// 
		// glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
		// glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
		// glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
		// glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

		glfwSetWindowMonitor(m_window_handle, monitor, m_window.position.x, m_window.position.y, m_window.size.x, m_window.size.y, 0);
	}

	void makeNonFullscreen() {
		glfwSetWindowMonitor(m_window_handle, NULL, m_window.position.x, m_window.position.y, m_window.size.x, m_window.size.y, 0);
	}



	// getters/setters
	float Window::aspect() { return m_aspect_ratio; };
	float Window::button(int button) { if (button >= M_NUM_BUTTONS || button < 0) oe_error_terminate("Invalid button {}", button); else return m_buttons[button]; }
	float Window::key(int key) { if (key >= M_NUM_KEYS || key < 0) oe_error_terminate("Invalid key {}", key); else return m_keys[key]; }

	const glm::vec2& Window::getPosition() { return m_window.position; }
	void Window::setPosition(const glm::vec2& pos) { m_window.position = pos; glfwSetWindowPos(m_window_handle, m_window.position.x, m_window.position.y); }

	const glm::vec2& Window::getSize() { return m_window.size; }
	void Window::setSize(const glm::vec2& size) { m_window.size = size; glfwSetWindowSize(m_window_handle, size.x, size.y); }

	const std::string& Window::getTitle() { return m_window.title; }
	void Window::setTitle(const std::string& title) { m_window.title = title; glfwSetWindowTitle(m_window_handle, title.c_str()); }

	bool Window::getBorderless() { return m_window.borderless; }
	void Window::setBorderless(bool borderless) { m_window.borderless = borderless; glfwSetWindowAttrib(m_window_handle, GLFW_DECORATED, !m_window.resizeable); }

	bool Window::getResizeable() { return m_window.resizeable; }
	void Window::setResizeable(bool resizeable) { m_window.resizeable = resizeable; glfwSetWindowAttrib(m_window_handle, GLFW_RESIZABLE, m_window.resizeable); }

	bool Window::getFullscreen() { return m_window.fullscreen; }
	void Window::setFullscreen(bool fullscreen) { m_window.fullscreen = fullscreen; m_window.fullscreen ? makeFullscreen() : makeNonFullscreen(); }

	const glm::vec2& Window::getCursorWindow() { return m_cursor_window; }
	void Window::setCursorWindow(const glm::vec2& cursor_at_window) { m_cursor_window = cursor_at_window; glfwSetCursorPos(m_window_handle, m_cursor_window.x, m_cursor_window.y); }

	const glm::vec2& Window::getCursorTransformed() { return m_cursor_transformed; }
	void Window::setCursorTransformed(const glm::vec2& cursor_at_world_space) {
		m_cursor_transformed = cursor_at_world_space;
		m_cursor_transformed.x = oe::utils::map((double)m_cursor_transformed.x, (double)-m_aspect_ratio, (double)m_aspect_ratio, (double)0.0, (double)m_window.size.x);
		m_cursor_transformed.y = oe::utils::map((double)m_cursor_transformed.y, (double)-1.0, (double)1.0, (double)0.0, (double)m_window.size.y);
		glfwSetCursorPos(m_window_handle, m_cursor_transformed.x, m_cursor_transformed.y);
	}
}
