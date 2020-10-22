#include "window.hpp"

#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

#include "engine/internal_libs.hpp"
#include "engine/utility/extra.hpp"
#include "engine/utility/connect_guard.hpp"
#include "engine/engine.hpp"



int oe_glfw_decorated = GLFW_DECORATED;
int oe_glfw_resizeable = GLFW_RESIZABLE;

namespace oe::graphics
{
	void resize_viewport(const oe::ResizeEvent& e)
	{
		glViewport(0, 0, e.framebuffer_size.x, e.framebuffer_size.y);
	}

	void IWindow::postglfw() 
	{
		glfwSetWindowUserPointer(m_window_handle, this);

		glfwSetCharModsCallback(m_window_handle, [](GLFWwindow* window, unsigned int codepoint, int mods) 
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				oe::CodepointEvent event;
				event.codepoint = codepoint;

				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				// if (this_class->m_window_info.text_callback) this_class->m_window_info.text_callback(static_cast<uint32_t>(codepoint), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetFramebufferSizeCallback(m_window_handle, [](GLFWwindow* window, int width, int height)                          
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				if (height < 1) height = 1;

				glm::vec2 old = this_class->m_window_info.size;

				this_class->m_window_info.size.x = width; this_class->m_window_info.size.y = height;
				this_class->m_aspect_ratio = (float)width / (float)height;

				oe::ResizeEvent event;
				event.framebuffer_size = this_class->m_window_info.size;
				event.framebuffer_size_old = old;
				event.aspect = this_class->getAspect();
				
				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().clear<oe::ResizeEvent>();
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				//if (this_class->m_window_info.resize_callback) this_class->m_window_info.resize_callback(this_class->m_window_info.size);
			});
		
		glfwSetCursorPosCallback(m_window_handle, [](GLFWwindow* window, double x, double y)
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				this_class->m_cursor_window = { x, y };

				float fx = oe::utils::map(static_cast<float>(x), 0.0f, static_cast<float>(this_class->m_window_info.size.x), -this_class->m_aspect_ratio, this_class->m_aspect_ratio);
				float fy = oe::utils::map(static_cast<float>(y), 0.0f, static_cast<float>(this_class->m_window_info.size.y), -1.0f, 1.0f);
				this_class->m_cursor_transformed = { fx, fy };

				oe::CursorPosEvent event;
				event.cursor_windowspace = this_class->m_cursor_window;
				event.cursor_worldspace = this_class->m_cursor_transformed;

				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().clear<oe::CursorPosEvent>(); // should multiple cursor pos events be sent to the application per frame 
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				// if (this_class->m_window_info.cursor_callback) this_class->m_window_info.cursor_callback(this_class->m_cursor_transformed, this_class->m_cursor_window);
			});
		
		glfwSetMouseButtonCallback(m_window_handle, [](GLFWwindow* window, int button, int action, int mods)
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				if (!oe::utils::isInRange(button, 0, max_number_of_buttons))
					return;

				if (action == GLFW_PRESS) this_class->m_buttons[button] = true;
				else if (action == GLFW_RELEASE) this_class->m_buttons[button] = false;

				oe::MouseButtonEvent event;
				event.button = static_cast<oe::mouse_buttons>(button);
				event.action = static_cast<oe::actions>(action);
				event.mods = static_cast<oe::modifiers>(mods);
				event.cursor_pos.cursor_windowspace = this_class->m_cursor_window;
				event.cursor_pos.cursor_worldspace = this_class->m_cursor_transformed;

				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				// if (this_class->m_window_info.button_callback) this_class->m_window_info.button_callback(static_cast<oe::mouse_buttons>(button), static_cast<oe::actions>(action));
			});
		
		glfwSetKeyCallback(m_window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)    
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				if (!oe::utils::isInRange(key, 0, max_number_of_keys))
					return;

				if (action == GLFW_PRESS) this_class->m_keys[key] = true;
				else if (action == GLFW_RELEASE) this_class->m_keys[key] = false;

				oe::KeyboardEvent event;
				event.key = static_cast<oe::keys>(key);
				event.action = static_cast<oe::actions>(action);
				event.mods = static_cast<oe::modifiers>(mods);

				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				// if (this_class->m_window_info.key_callback) this_class->m_window_info.key_callback(static_cast<oe::keys>(key), static_cast<oe::actions>(action), static_cast<oe::modifiers>(mods));
			});
		
		glfwSetScrollCallback(m_window_handle, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				IWindow* this_class = reinterpret_cast<IWindow*>(glfwGetWindowUserPointer(window));

				oe::ScrollEvent event;
				event.scroll_delta = { xoffset, yoffset };

				this_class->dispatcher_mutex.lock();
				this_class->m_window_gameloop.getDispatcher().enqueue(event);
				this_class->dispatcher_mutex.unlock();
				// if (this_class->m_window_info.scroll_callback) this_class->m_window_info.scroll_callback(yoffset);
			});

		// to call resize func initially
		glfwSetWindowSizeLimits(m_window_handle, 1, 1, INT16_MAX, INT16_MAX);
		glfwSetWindowSize(m_window_handle, m_window_info.size.x, m_window_info.size.y);

		swapInterval(m_window_info.swap_interval);

		connect_listener<oe::ResizeEvent, &resize_viewport>();
	}

	IWindow::IWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config) 
		: m_window_info(window_config)
		, m_window_gameloop(this)
	{
		m_window_info.size.y = std::max(m_window_info.size.y, static_cast<uint32_t>(1));
		m_aspect_ratio = static_cast<float>(m_window_info.size.x) / static_cast<float>(m_window_info.size.y);

		m_keys.fill(false);
		m_buttons.fill(false);
	}

	IWindow::~IWindow() 
	{

	}



	bool IWindow::shouldClose() 
	{
		return glfwWindowShouldClose(m_window_handle);
	}

	void IWindow::setIcon(const oe::utils::image_data& image) 
	{
		GLFWimage glfwicon; 
		glfwicon.height = image.height; glfwicon.width = image.width; glfwicon.pixels = const_cast<unsigned char*>(image.data);
		glfwSetWindowIcon(m_window_handle, 1, &glfwicon);
	}

	void IWindow::showCursor(bool show) 
	{
		if (!show) glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else glfwSetInputMode(m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void IWindow::makeFullscreen() 
	{
		glfwSetWindowMonitor(m_window_handle, glfwGetPrimaryMonitor(), m_window_info.position.x, m_window_info.position.y, m_window_info.size.x, m_window_info.size.y, 0);
	}

	void IWindow::makeWindowed() 
	{
		glfwSetWindowMonitor(m_window_handle, NULL, m_window_info.position.x, m_window_info.position.y, m_window_info.size.x, m_window_info.size.y, 0);
	}

}
