#include "window.h"

#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

#include "engine/internal_libs.h"
#include "engine/utility/math.h"
#include "engine/engine.h"



namespace oe::graphics {

	void Window::postglfw() {
		glfwSetWindowUserPointer(m_window_handle, this);

		glfwSetCharModsCallback(m_window_handle, [](GLFWwindow* window, unsigned int codepoint, int mods) 
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (this_class->m_text_callback) this_class->m_text_callback(static_cast<uint32_t>(codepoint), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetFramebufferSizeCallback(m_window_handle, [](GLFWwindow* window, int width, int height)                          
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (height < 1) height = 1;

				this_class->m_window.size.x = width; this_class->m_window.size.y = height;
				this_class->m_aspect_ratio = (float)width / (float)height;
				glViewport(0, 0, this_class->m_window.size.x, this_class->m_window.size.y);

				if (this_class->m_resize_callback) this_class->m_resize_callback(this_class->m_window.size);
			});
		
		glfwSetCursorPosCallback(m_window_handle, [](GLFWwindow* window, double x, double y)                             
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				this_class->m_cursor_window = { x, y };

				x = oe::utils::map((double)x, (double)0.0, (double)this_class->m_window.size.x, (double)-this_class->m_aspect_ratio, (double)this_class->m_aspect_ratio);
				y = oe::utils::map((double)y, (double)0.0, (double)this_class->m_window.size.y, (double)-1.0, (double)1.0);
				this_class->m_cursor_transformed = { x, y };

				if (this_class->m_cursor_callback) this_class->m_cursor_callback(this_class->m_cursor_window, this_class->m_cursor_transformed);
			});
		
		glfwSetMouseButtonCallback(m_window_handle, [](GLFWwindow* window, int button, int action, int mods)               
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (button >= M_NUM_BUTTONS || button < 0) return;

				if (action == GLFW_PRESS) this_class->m_buttons[button] = true;
				else if (action == GLFW_RELEASE) this_class->m_buttons[button] = false;

				if (this_class->m_button_callback) this_class->m_button_callback(static_cast<oe::mouse>(button), static_cast<oe::actions>(action));
			});
		
		glfwSetKeyCallback(m_window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)    
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (key >= M_NUM_KEYS || key < 0) return;

				if (action == GLFW_PRESS) this_class->m_keys[key] = true;
				else if (action == GLFW_RELEASE) this_class->m_keys[key] = false;

				if (this_class->m_key_callback) this_class->m_key_callback(static_cast<oe::keyboard>(key), static_cast<oe::actions>(action), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetScrollCallback(m_window_handle, [](GLFWwindow* window, double xoffset, double yoffset)                 
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (this_class->m_scroll_callback) this_class->m_scroll_callback(yoffset);
			});

		// to call resize func initially
		glfwSetWindowSize(m_window_handle, m_window.size.x, m_window.size.y);
	}

	Window::Window(const WindowInfo& window_config) {
		m_window = window_config;
		m_window.size.y = std::max(m_window.size.y, 1.0f);
		m_aspect_ratio = m_window.size.x / m_window.size.y;
	}

	Window::~Window() {
		glfwDestroyWindow(m_window_handle);
	}



	bool Window::shouldClose() {
		return glfwWindowShouldClose(m_window_handle);
	}

	void Window::setIcon(const oe::utils::image_data& image) {
		GLFWimage glfwicon; 
		glfwicon.height = image.height; glfwicon.width = image.width; glfwicon.pixels = const_cast<unsigned char*>(image.data);
		glfwSetWindowIcon(m_window_handle, 1, &glfwicon);
	}

	void Window::showCursor(bool show) {
		if (!show) glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}



	void Window::setKeyboardCallback(key_callback callback) { m_key_callback = callback; }
	void Window::setButtonCallback(button_callback callback) { m_button_callback = callback; }
	void Window::setScrollCallback(scroll_callback callback) { m_scroll_callback = callback; }
	void Window::setResizeCallback(resize_callback callback) { m_resize_callback = callback; }
	void Window::setCharmodCallback(text_callback callback) { m_text_callback = callback; }
	void Window::setCursorPositionCallback(cursor_callback callback) { m_cursor_callback = callback; }



	void makeFullscreen(GLFWwindow* window_handle, const WindowInfo& window) {
		glfwSetWindowMonitor(window_handle, glfwGetPrimaryMonitor(), window.position.x, window.position.y, window.size.x, window.size.y, 0);
	}

	void makeWindowed(GLFWwindow* window_handle, const WindowInfo& window) {
		glfwSetWindowMonitor(window_handle, NULL, window.position.x, window.position.y, window.size.x, window.size.y, 0);
	}



	// getters/setters
	float Window::aspect() { return m_aspect_ratio; };
	float Window::button(oe::mouse button) { if (button >= M_NUM_BUTTONS || button < 0) oe_error_terminate("Invalid button {}", button); else return m_buttons[button]; }
	float Window::key(oe::keyboard key) { if (key >= M_NUM_KEYS || key < 0) oe_error_terminate("Invalid key {}", key); else return m_keys[key]; }

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
	void Window::setFullscreen(bool fullscreen) { m_window.fullscreen = fullscreen; m_window.fullscreen ? makeFullscreen(m_window_handle, m_window) : makeWindowed(m_window_handle, m_window); }

	const glm::vec2& Window::getCursorWindow() { return m_cursor_window; }
	void Window::setCursorWindow(const glm::vec2& cursor_at_window) { m_cursor_window = cursor_at_window; glfwSetCursorPos(m_window_handle, m_cursor_window.x, m_cursor_window.y); }

	const glm::vec2& Window::getCursorTransformed() { return m_cursor_transformed; }
	void Window::setCursorTransformed(const glm::vec2& cursor_at_world_space) {
		m_cursor_transformed = cursor_at_world_space;
		m_cursor_transformed.x = oe::utils::map((double)m_cursor_transformed.x, (double)-m_aspect_ratio, (double)m_aspect_ratio, (double)0.0, (double)m_window.size.x);
		m_cursor_transformed.y = oe::utils::map((double)m_cursor_transformed.y, (double)-1.0, (double)1.0, (double)0.0, (double)m_window.size.y);
		glfwSetCursorPos(m_window_handle, m_cursor_transformed.x, m_cursor_transformed.y);
	}

	const std::string Window::getClipboard() {
		return glfwGetClipboardString(nullptr);
	}
	
	void Window::setClipboard(const std::string& str) {
		glfwSetClipboardString(nullptr, str.c_str());
	}

}
