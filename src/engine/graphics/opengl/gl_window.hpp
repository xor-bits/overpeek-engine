#pragma once

#include <string>

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/window.hpp"

#include "engine/graphics/opengl/gl_framebuffer.hpp"
#include "engine/graphics/opengl/gl_shader.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"



namespace oe::graphics
{	
	class GLWindow : public IWindow
	{
	private:
		bool m_debugging;

	public:
		GLWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config);
		~GLWindow();

		void glad(const std::unique_ptr<Instance>& instance);
		void glfw(const std::unique_ptr<Instance>& instance);

		// Inherited via Window
		void update() override;
		void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		void viewport() const override;
		void swapInterval(uint8_t frames) override;
		void bind() const override;

		void pollEvents() override;
		void waitEvents(float timeout) override;
		void updateEvents() override;
		void bump() override;

		void active_context() const override;
		void inactive_context() const override;

		std::string getAPI() const override;
		std::string getAPIVersion() const override;
		std::string getGPU() const override;
		std::string getGPUVendor() const override;
	};

}