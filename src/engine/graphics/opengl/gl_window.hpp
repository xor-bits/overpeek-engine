#pragma once

#include <string>

#include "engine/utility/filereader.hpp"
#include "engine/internal_libs.hpp"

#include "engine/graphics/interface/window.hpp"



namespace oe::graphics {
	
	class GLWindow : public Window {
	private:
		bool m_debugging;

	public:
		GLWindow(const Instance* instance, const WindowInfo& window_config);
		~GLWindow();

		void glad(const Instance* instance);
		void glfw(const Instance* instance);

		// Inherited via Window
		virtual void update() override;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		virtual void viewport() override;
		virtual void swapInterval(uint8_t frames) override;
	};

}