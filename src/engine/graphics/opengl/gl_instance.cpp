#include "gl_instance.hpp"
#include "gl_window.hpp"
#include "gl_shader.hpp"
#include "gl_texture.hpp"
#include "gl_framebuffer.hpp"

#include "engine/engine.hpp"

#include "gl_include.hpp"
#include <GLFW/glfw3.h>



namespace oe::graphics
{
	int get_gl_version_number()
	{
		int gl_v_major, gl_v_minor;
		glGetIntegerv(GL_MAJOR_VERSION, &gl_v_major);
		glGetIntegerv(GL_MINOR_VERSION, &gl_v_minor);

		return gl_v_major * 10 + gl_v_minor;
	}

	GLInstance::GLInstance() 
		: Instance() 
	{}

	GLInstance::~GLInstance()
	{}



	void GLInstance::blending(oe::modes mode) const
	{
		switch (mode)
		{
		case oe::modes::enable:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case oe::modes::disable:
			glDisable(GL_BLEND);
			break;
		default: // keep
			break;
		}
	}

	void GLInstance::depth(oe::depth_functions func) const
	{
		glDepthMask(GL_TRUE);
		switch (func)
		{
		case oe::depth_functions::disable:
			// fall through
			[[fallthrough]];
		case oe::depth_functions::always:
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_ALWAYS);
			break;
		case oe::depth_functions::never:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_NEVER);
			break;
		case oe::depth_functions::less_than:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			break;
		case oe::depth_functions::greater_than:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GREATER);
			break;
		case oe::depth_functions::equal:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_EQUAL);
			break;
		case oe::depth_functions::less_than_or_equal:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			break;
		case oe::depth_functions::greater_than_or_equal:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_GEQUAL);
			break;
		}
	}

	void GLInstance::swapInterval(unsigned int interval) const
	{
		glfwSwapInterval(interval);
	}

	void GLInstance::culling(oe::culling_modes c) const
	{
		switch (c)
		{
		case oe::culling_modes::neither:
			glDisable(GL_CULL_FACE);
			break;
		case oe::culling_modes::both:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			glFrontFace(GL_CCW);
			break;
		case oe::culling_modes::front:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
			break;
		case oe::culling_modes::back:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			break;
		}
	}

	void GLInstance::lineWidth(float w) const
	{
		glLineWidth(w);
	}

	void GLInstance::pointRadius(float w) const
	{
		glPointSize(w);
	}

	void GLInstance::polygonMode(oe::polygon_mode mode) const
	{
		switch (mode)
		{
		case oe::polygon_mode::fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case oe::polygon_mode::lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case oe::polygon_mode::points:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		default:
			break;
		}
	}

	void GLInstance::viewport(int x, int y, size_t w, size_t h) const
	{
		int gl_w = static_cast<int>(std::clamp<size_t>(w, 0, static_cast<size_t>(std::numeric_limits<int>::max())));
		int gl_h = static_cast<int>(std::clamp<size_t>(h, 0, static_cast<size_t>(std::numeric_limits<int>::max())));
		glViewport(x, y, gl_w, gl_h);
	}

	int GLInstance::versionNumber()
	{
		if (m_version_number == -1) m_version_number = get_gl_version_number();
		return m_version_number;
	}

}