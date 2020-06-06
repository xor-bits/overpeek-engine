#include "window.hpp"

#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

#include "engine/internal_libs.hpp"
#include "engine/utility/extra.hpp"
#include "engine/engine.hpp"



namespace oe::graphics {

	void Window::postglfw() 
	{
		glfwSetWindowUserPointer(m_window_handle, this);

		glfwSetCharModsCallback(m_window_handle, [](GLFWwindow* window, unsigned int codepoint, int mods) 
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				oe::CodepointEvent event;
				event.codepoint = codepoint;
				this_class->dispatcher.trigger(event);
				// if (this_class->m_window_info.text_callback) this_class->m_window_info.text_callback(static_cast<uint32_t>(codepoint), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetFramebufferSizeCallback(m_window_handle, [](GLFWwindow* window, int width, int height)                          
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
				this_class->active_context();

				if (height < 1) height = 1;

				glm::vec2 old = this_class->m_window_info.size;

				this_class->m_window_info.size.x = width; this_class->m_window_info.size.y = height;
				this_class->m_aspect_ratio = (float)width / (float)height;
				glViewport(0, 0, this_class->m_window_info.size.x, this_class->m_window_info.size.y);

				oe::ResizeEvent event;
				event.framebuffer_size = this_class->m_window_info.size;
				event.framebuffer_size_old = old;
				event.aspect = this_class->aspect();
				this_class->dispatcher.trigger(event);
				//if (this_class->m_window_info.resize_callback) this_class->m_window_info.resize_callback(this_class->m_window_info.size);
			});
		
		glfwSetCursorPosCallback(m_window_handle, [](GLFWwindow* window, double x, double y)
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				this_class->m_cursor_window = { x, y };

				x = oe::utils::map(static_cast<float>(x), 0.0f, this_class->m_window_info.size.x, -this_class->m_aspect_ratio, this_class->m_aspect_ratio);
				y = oe::utils::map(static_cast<float>(y), 0.0f, this_class->m_window_info.size.y, -1.0f, 1.0f);
				this_class->m_cursor_transformed = { x, y };

				oe::CursorPosEvent event;
				event.cursor_windowspace = this_class->m_cursor_window;
				event.cursor_worldspace = this_class->m_cursor_transformed;
				this_class->dispatcher.trigger(event);
				// if (this_class->m_window_info.cursor_callback) this_class->m_window_info.cursor_callback(this_class->m_cursor_transformed, this_class->m_cursor_window);
			});
		
		glfwSetMouseButtonCallback(m_window_handle, [](GLFWwindow* window, int button, int action, int mods)
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (button >= M_NUM_BUTTONS || button < 0) return;

				if (action == GLFW_PRESS) this_class->m_buttons[button] = true;
				else if (action == GLFW_RELEASE) this_class->m_buttons[button] = false;

				oe::MouseButtonEvent event;
				event.button = static_cast<oe::mouse_buttons>(button);
				event.action = static_cast<oe::actions>(action);
				event.mods = static_cast<oe::modifiers>(mods);
				this_class->dispatcher.trigger(event);
				// if (this_class->m_window_info.button_callback) this_class->m_window_info.button_callback(static_cast<oe::mouse_buttons>(button), static_cast<oe::actions>(action));
			});
		
		glfwSetKeyCallback(m_window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)    
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				if (key >= M_NUM_KEYS || key < 0) return;

				if (action == GLFW_PRESS) this_class->m_keys[key] = true;
				else if (action == GLFW_RELEASE) this_class->m_keys[key] = false;

				oe::KeyboardEvent event;
				event.key = static_cast<oe::keys>(key);
				event.action = static_cast<oe::actions>(action);
				event.mods = static_cast<oe::modifiers>(mods);
				this_class->dispatcher.trigger(event);
				// if (this_class->m_window_info.key_callback) this_class->m_window_info.key_callback(static_cast<oe::keys>(key), static_cast<oe::actions>(action), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetScrollCallback(m_window_handle, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				Window* this_class = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

				oe::ScrollEvent event;
				event.scroll_delta = { xoffset, yoffset };
				this_class->dispatcher.trigger(event);
				// if (this_class->m_window_info.scroll_callback) this_class->m_window_info.scroll_callback(yoffset);
			});

		// to call resize func initially
		glfwSetWindowSizeLimits(m_window_handle, 1, 1, INT16_MAX, INT16_MAX);
		glfwSetWindowSize(m_window_handle, m_window_info.size.x, m_window_info.size.y);

		swapInterval(m_window_info.swap_interval);
	}

	Window::Window(const Instance* instance, const WindowInfo& window_config) 
		: m_instance(instance)
		, m_window_info(window_config)
		, m_window_gameloop(this, m_window_info.main_updatesystem_ups)
	{
		m_window_info.size.y = std::max(m_window_info.size.y, 1.0f);
		m_aspect_ratio = m_window_info.size.x / m_window_info.size.y;

		for (size_t i = 0; i < M_NUM_KEYS; i++)
		{
			m_keys[i] = false;
		}
		for (size_t i = 0; i < M_NUM_BUTTONS; i++)
		{
			m_buttons[i] = false;
		}
	}

	Window::~Window() 
	{

	}



	bool Window::shouldClose() 
	{
		return glfwWindowShouldClose(m_window_handle);
	}

	void Window::setIcon(const oe::utils::image_data& image) 
	{
		GLFWimage glfwicon; 
		glfwicon.height = image.height; glfwicon.width = image.width; glfwicon.pixels = const_cast<unsigned char*>(image.data);
		glfwSetWindowIcon(m_window_handle, 1, &glfwicon);
	}

	void Window::showCursor(bool show) 
	{
		if (!show) glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}



	void makeFullscreen(GLFWwindow* window_handle, const WindowInfo& window) 
	{
		glfwSetWindowMonitor(window_handle, glfwGetPrimaryMonitor(), window.position.x, window.position.y, window.size.x, window.size.y, 0);
	}

	void makeWindowed(GLFWwindow* window_handle, const WindowInfo& window) 
	{
		glfwSetWindowMonitor(window_handle, NULL, window.position.x, window.position.y, window.size.x, window.size.y, 0);
	}



	// getters/setters
	float Window::aspect() { return m_aspect_ratio; };
	float Window::button(oe::mouse_buttons button) { int32_t num = static_cast<int32_t>(button); if (num >= M_NUM_BUTTONS || num < 0) oe_error_terminate("Invalid button {}", button); else return m_buttons[num]; }
	float Window::key(oe::keys key) { int32_t num = static_cast<int32_t>(key); if (num >= M_NUM_KEYS || num < 0) oe_error_terminate("Invalid key {}", key); else return m_keys[num]; }

	const glm::vec2& Window::getPosition() { return m_window_info.position; }
	void Window::setPosition(const glm::vec2& pos) { m_window_info.position = pos; glfwSetWindowPos(m_window_handle, m_window_info.position.x, m_window_info.position.y); }

	const glm::vec2& Window::getSize() { return m_window_info.size; }
	void Window::setSize(const glm::vec2& size) { m_window_info.size = size; glfwSetWindowSize(m_window_handle, size.x, size.y); }

	const std::string& Window::getTitle() { return m_window_info.title; }
	void Window::setTitle(const std::string& title) { m_window_info.title = title; glfwSetWindowTitle(m_window_handle, title.c_str()); }

	bool Window::getBorderless() { return m_window_info.borderless; }
	void Window::setBorderless(bool borderless) { m_window_info.borderless = borderless; glfwSetWindowAttrib(m_window_handle, GLFW_DECORATED, !m_window_info.resizeable); }

	bool Window::getResizeable() { return m_window_info.resizeable; }
	void Window::setResizeable(bool resizeable) { m_window_info.resizeable = resizeable; glfwSetWindowAttrib(m_window_handle, GLFW_RESIZABLE, m_window_info.resizeable); }

	bool Window::getFullscreen() { return m_window_info.fullscreen; }
	void Window::setFullscreen(bool fullscreen) { m_window_info.fullscreen = fullscreen; m_window_info.fullscreen ? makeFullscreen(m_window_handle, m_window_info) : makeWindowed(m_window_handle, m_window_info); }

	const glm::vec2& Window::getCursorWindow() { return m_cursor_window; }
	void Window::setCursorWindow(const glm::vec2& cursor_at_window) { m_cursor_window = cursor_at_window; glfwSetCursorPos(m_window_handle, m_cursor_window.x, m_cursor_window.y); }

	const glm::vec2& Window::getCursorTransformed() { return m_cursor_transformed; }
	void Window::setCursorTransformed(const glm::vec2& cursor_at_world_space) 
	{
		m_cursor_transformed = cursor_at_world_space;
		m_cursor_transformed.x = oe::utils::map((float)m_cursor_transformed.x, (float)-m_aspect_ratio, (float)m_aspect_ratio, (float)0.0, (float)m_window_info.size.x);
		m_cursor_transformed.y = oe::utils::map((float)m_cursor_transformed.y, (float)-1.0, (float)1.0, (float)0.0, (float)m_window_info.size.y);
		glfwSetCursorPos(m_window_handle, m_cursor_transformed.x, m_cursor_transformed.y);
	}

	const std::string Window::getClipboard() { return glfwGetClipboardString(m_window_handle); }
	
	void Window::setClipboard(const std::string& str) {	glfwSetClipboardString(m_window_handle, str.c_str()); }

}
