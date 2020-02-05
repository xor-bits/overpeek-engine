#include "gl_window.h"

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/internal_libs.h"
#include "engine/utility/math.h"
#include "engine/graphics/opengl/gl.h"
#include "engine/engine.h"

#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024





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

	void GLWindow::glfw() {
		glfwSetErrorCallback(glfwError);
		if (!glfwInit()) oe_error_terminate("Failed to initialize GLFW!");

		// Window hints
		glfwWindowHint(GLFW_SAMPLES, m_window.multisamples);
		glfwWindowHint(GLFW_RESIZABLE, m_window.resizeable);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_window.transparent);
		glfwWindowHint(GLFW_DECORATED, !m_window.borderless);
		if (m_window.debug_messages) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

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

	void GLWindow::glad() {
		// Init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) oe_error_terminate("Failed to init glad");

		// GL config
		if (m_window.multisamples) glEnable(GL_MULTISAMPLE);
		if (m_window.debug_messages) {
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		// GL
		viewport();
		GL::setPolygonMode();
		GL::setSwapInterval();
		GL::setCulling();
		GL::enableDepth();
		GL::enableBlending();
	}

	GLWindow::GLWindow(WindowInfo& window_config) : Window::Window(window_config) {
		spdlog::info("Opening window with OpenGL context");

		glfw();
		glad();

		Window::postglfw();
	}

	GLWindow::~GLWindow() {

	}



	void GLWindow::update() {
		glfwPollEvents();
		glfwSwapBuffers(m_window_handle);
	}

	void GLWindow::clear(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLWindow::viewport() {
		glViewport(0, 0, m_window.size.x, m_window.size.y);
	}

}
