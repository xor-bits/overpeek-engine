#include "gl_window.hpp"

#include <string>
#include <iostream>

#include "gl_include.hpp"
#include <GLFW/glfw3.h>

#include "engine/internal_libs.hpp"
#include "engine/utility/extra.hpp"
#include "engine/engine.hpp"



#ifndef __EMSCRIPTEN__
void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* /* userParam */) {
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
		case GL_DEBUG_SOURCE_SHADER_COMPILER: log_source = "IShader Compiler"; break;
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

	std::cout << fmt::format("Message       :   ({}): {}", id, std::string_view(message, length)) << std::endl;
	std::cout << fmt::format("Source        :   {}", log_source) << std::endl;
	std::cout << fmt::format("Description   :   {}", log_type) << std::endl;
	std::cout << fmt::format("Line          :   {}", log_severity) << std::endl;
	std::cout << std::endl;


	constexpr const std::string_view formatted_error = "OpenAL error";
	if(oe::Engine::getSingleton().engine_info.ignore_errors && severity == GL_DEBUG_SEVERITY_HIGH)
		spdlog::warn("{}", formatted_error);
	else
		throw std::runtime_error(std::string{ formatted_error });
}
#endif

namespace oe::graphics {

	void GLWindow::glfw(const std::unique_ptr<Instance>& /* instance */) {
		// Window hints
		glfwWindowHint(GLFW_SAMPLES, m_window_info.multisamples);
		glfwWindowHint(GLFW_RESIZABLE, m_window_info.resizeable);
		glfwWindowHint(GLFW_DECORATED, !m_window_info.borderless);
		if (m_debugging) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

#ifndef __EMSCRIPTEN__ // no transparency with webgl
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_window_info.transparent);
#endif /* __EMSCRIPTEN__ */

		// Window creation
		GLFWwindow* share_handle = m_window_info.share_handle != nullptr ? ((GLWindow*)m_window_info.share_handle)->m_window_handle : nullptr;
		m_window_handle = glfwCreateWindow(m_window_info.size.x, m_window_info.size.y, m_window_info.title.data(), m_window_info.fullscreen ? glfwGetPrimaryMonitor() : NULL, share_handle);
		if (!m_window_handle)
			throw std::runtime_error("Failed to create window!");

#ifndef __EMSCRIPTEN__ // window is stuck in the browser so it cant move
		//Center the window
		int monitor_xpos, monitor_ypos, monitor_width, monitor_height;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitor_xpos, &monitor_ypos, &monitor_width, &monitor_height);
		m_window_info.position = { (monitor_width - m_window_info.size.x) * 0.5f, (monitor_height - m_window_info.size.y) * 0.5f };
		glfwSetWindowPos(m_window_handle, m_window_info.position.x, m_window_info.position.y);
#endif /* __EMSCRIPTEN__ */

		// set context for opengl
		active_context();
	}

	void GLWindow::glad(const std::unique_ptr<Instance>& instance) {
		// Init glad
#ifndef __EMSCRIPTEN__
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to init glad");
#endif

		int gl_v_major, gl_v_minor;
		glGetIntegerv(GL_MAJOR_VERSION, &gl_v_major);
		glGetIntegerv(GL_MINOR_VERSION, &gl_v_minor);
		std::string gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		std::string glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
		spdlog::info("OpenGL: {}", gl_version);
		spdlog::info("OpenGL Shading Language: {}", glsl_version);

		// GL config
#ifndef __EMSCRIPTEN__
		if (m_window_info.multisamples) glEnable(GL_MULTISAMPLE);
#endif
		if (m_debugging) {
#ifndef __EMSCRIPTEN__
			if (((gl_v_major == 4 && gl_v_minor >= 3) || gl_v_major > 4)) {
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			} else {
				spdlog::critical("OpenGL {}.{} does not support glDebugMessageCallback", gl_v_major, gl_v_minor);
			}
#else
			spdlog::critical("EMSCRIPTEN does not support glDebugMessageCallback");
#endif
		}

		// GL
		viewport();
		instance->polygonMode();
		instance->swapInterval();
		instance->culling();
		instance->depth();
		instance->blending();
	}

	GLWindow::GLWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config) 
		: IWindow::IWindow(instance, window_config)
		, m_debugging(oe::Engine::getSingleton().engine_info.debug_mode)
	{
		oe_debug_call("gl_window");

		if (m_debugging) {
			spdlog::warn("Debugger enabled");
			spdlog::info("Opening window with OpenGL context");
		}

		glfw(instance);
		glad(instance);

		IWindow::postglfw();
	}

	GLWindow::~GLWindow() 
	{
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
	}



	void GLWindow::pollEvents()
	{
		glfwPollEvents();
		updateEvents();
	}

	void GLWindow::waitEvents(float timeout)
	{
		if (timeout == 0.0f)
			glfwWaitEvents();
		else
			glfwWaitEventsTimeout(timeout);
	}

	void GLWindow::updateEvents()
	{
		m_processing_events = true;
		dispatcher_mutex.lock();
		m_window_gameloop.getDispatcher().update();
		dispatcher_mutex.unlock();
		m_processing_events = false;
	}

	void GLWindow::bump()
	{
		glfwPostEmptyEvent();
	}

	void GLWindow::update() 
	{
		glfwSwapBuffers(m_window_handle);
	}

	void GLWindow::clear(const glm::vec4& color)
	{
		bind();

		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLWindow::viewport() const
	{
		glViewport(0, 0, m_window_info.size.x, m_window_info.size.y);
	}

	void GLWindow::swapInterval(uint8_t frames) 
	{
		glfwSwapInterval(frames);
	}

	void GLWindow::active_context() const {
		// set context for opengl
		glfwMakeContextCurrent(m_window_handle);
	}
	
	void GLWindow::inactive_context() const {
		glfwMakeContextCurrent(NULL);
	}
	
	void GLWindow::bind() const
	{
		if (GLFrameBuffer::bound_fbo_id == 0) return;
		GLFrameBuffer::bound_fbo_id = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		viewport();
	}



	std::string GLWindow::getAPI() const
	{
		return "OpenGL";
	}

	std::string GLWindow::getAPIVersion() const
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	std::string GLWindow::getGPU() const
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}

	std::string GLWindow::getGPUVendor() const
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	}

}
