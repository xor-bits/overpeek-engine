#pragma once

#include <string>

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/window.hpp"

#include "engine/graphics/opengl/gl_framebuffer.hpp"
#include "engine/graphics/opengl/gl_shader.hpp"
#include "engine/graphics/opengl/gl_texture.hpp"



namespace oe::graphics {
	
	class GLWindow : public IWindow {
	private:
		bool m_debugging;

	public:
		GLWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config);
		~GLWindow();

		void glad(const std::unique_ptr<Instance>& instance);
		void glfw(const std::unique_ptr<Instance>& instance);

		// Inherited via Window
		virtual void update() override;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		virtual void viewport() const override;
		virtual void swapInterval(uint8_t frames) override;
		virtual void bind() const override;

		virtual void pollEvents() override;
		virtual void waitEvents(float timeout) override;
		virtual void updateEvents() override;
		virtual void bump() override;

		virtual void active_context() const override;
		virtual void inactive_context() const override;

		virtual std::string getAPI() const override;
		virtual std::string getAPIVersion() const override;
		virtual std::string getGPU() const override;
		virtual std::string getGPUVendor() const override;
	};

}