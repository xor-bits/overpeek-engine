#include "gl.h"

#include "engine/internal_libs.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



namespace oe::graphics {

	void GL::enableDepth(depth_func f) {
		glEnable(GL_DEPTH_TEST);
		switch (f)
		{
		case oe::graphics::GL::depth_func::always:
			glDepthFunc(GL_ALWAYS);
			break;
		case oe::graphics::GL::depth_func::never:
			glDepthFunc(GL_NEVER);
			break;
		case oe::graphics::GL::depth_func::less_than:
			glDepthFunc(GL_LESS);
			break;
		case oe::graphics::GL::depth_func::greater_than:
			glDepthFunc(GL_GREATER);
			break;
		case oe::graphics::GL::depth_func::equal:
			glDepthFunc(GL_EQUAL);
			break;
		case oe::graphics::GL::depth_func::less_than_or_equal:
			glDepthFunc(GL_LEQUAL);
			break;
		case oe::graphics::GL::depth_func::greater_than_or_equal:
			glDepthFunc(GL_GEQUAL);
			break;
		}
	}

	void GL::disableDepth() {
		glDisable(GL_DEPTH_TEST);
	}

	void GL::setLineWidth(float w) {
		glLineWidth(w);
	}

	void GL::setPointRadius(float w) {
		glPointSize(w);
	}

	void GL::setPolygonMode(polygon_mode mode) {
		switch (mode)
		{
		case oe::graphics::GL::polygon_mode::fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case oe::graphics::GL::polygon_mode::lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case oe::graphics::GL::polygon_mode::points:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		}
	}

	void GL::enableBlending() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GL::disableBlending() {
		glDisable(GL_BLEND);
	}

	void GL::setCulling(culling c) {
		switch (c)
		{
		case oe::graphics::GL::culling::neither:
			glDisable(GL_CULL_FACE);
			break;
		case oe::graphics::GL::culling::both:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			glFrontFace(GL_CCW);
			break;
		case oe::graphics::GL::culling::front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
			break;
		case oe::graphics::GL::culling::back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			break;
		}
	}

	void GL::setSwapInterval(unsigned int interval) {
		glfwSwapInterval(interval);
	}

	int GL::__checkGLErrors(const std::string& file, int line) {
		GLenum err = glGetError();
		if (err != 0) {
			std::string errorText;
			switch (err)
			{
			case GL_NO_ERROR:
				errorText = "How did you get an error while not getting an error?";
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
			spdlog::error("OpenGL Error");
			std::cout << fmt::format("Code          :   {}", err) << std::endl;
			std::cout << fmt::format("Description   :   {}", errorText) << std::endl;
			std::cout << fmt::format("Line          :   {}", line) << std::endl;
			std::cout << fmt::format("File          :   {}", file) << std::endl;
			std::cout << std::endl;
			oe::terminate();
			return err;
		}
		return 0;
	}

	std::string GL::getVersion() {
		return std::string((char*)glGetString(GL_VERSION));
	}

	std::string GL::getRenderer() {
		return std::string((char*)glGetString(GL_RENDERER));
	}

	std::string GL::getVendor() {
		return std::string((char*)glGetString(GL_VENDOR));
	}

}