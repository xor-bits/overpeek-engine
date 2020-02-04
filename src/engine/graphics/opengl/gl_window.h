#pragma once

#include <string>

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"

#include "../window.h"



namespace oe::graphics {
	
	class GLWindow : public Window {
	public:
		GLWindow(WindowInfo& window_config);
		~GLWindow();

		void glad();
		void glfw();

		// Inherited via Window
		virtual void update() override;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		virtual void viewport() override;
	};

}