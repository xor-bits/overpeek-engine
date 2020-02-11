#include "gl_window.hpp"

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/internal_libs.hpp"
#include "engine/utility/math.hpp"
#include "engine/engine.hpp"



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
	oe_error_terminate("OpenGL error");
}

void glfwError(int id, const char* description) {
	oe_error_terminate("GLFW ({}): {}", id, description);
}

namespace oe::graphics {

	void GLWindow::glfw(const Instance* instance) {
		glfwSetErrorCallback(glfwError);
		if (!glfwInit()) oe_error_terminate("Failed to initialize GLFW!");

		// Window hints
		glfwWindowHint(GLFW_SAMPLES, m_window_info.multisamples);
		glfwWindowHint(GLFW_RESIZABLE, m_window_info.resizeable);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_window_info.transparent);
		glfwWindowHint(GLFW_DECORATED, !m_window_info.borderless);
		if (m_debugging) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		// Window creation
		m_window_handle = glfwCreateWindow(m_window_info.size.x, m_window_info.size.y, m_window_info.title.c_str(), m_window_info.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!m_window_handle) oe_error_terminate("Failed to create window!");

		//Center the window
		int monitor_xpos, monitor_ypos, monitor_width, monitor_height;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);
		m_window_info.position = { (monitor_width - m_window_info.size.x) * 0.5f, (monitor_height - m_window_info.size.y) * 0.5f };
		glfwSetWindowPos(m_window_handle, m_window_info.position.x, m_window_info.position.y);

		// set context for opengl
		glfwMakeContextCurrent(m_window_handle);
	}

	void GLWindow::glad(const Instance* instance) {
		// Init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) oe_error_terminate("Failed to init glad");

		// GL config
		if (m_window_info.multisamples) glEnable(GL_MULTISAMPLE);
		if (m_debugging) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		// GL
		viewport();
		instance->polygonMode();
		instance->swapInterval();
		instance->culling();
		instance->depth();
		instance->blending();
	}

	GLWindow::GLWindow(const Instance* instance, const WindowInfo& window_config) 
		: Window::Window(instance, window_config)
		, m_debugging(instance->m_instance_info.debug_messages) 
	{
		spdlog::info("Opening window with OpenGL context");
		if (m_debugging)
			spdlog::warn("Debugger enabled");

		glfw(instance);
		glad(instance);

		Window::postglfw();
	}

	GLWindow::~GLWindow() 
	{
		spdlog::info("window termination");
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}



	void GLWindow::update() 
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window_handle);
	}

	void GLWindow::clear(const glm::vec4& color) 
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLWindow::viewport() 
	{
		glViewport(0, 0, m_window_info.size.x, m_window_info.size.y);
	}

	void GLWindow::swapInterval(uint8_t frames) 
	{
		glfwSwapInterval(frames);
	}

}
