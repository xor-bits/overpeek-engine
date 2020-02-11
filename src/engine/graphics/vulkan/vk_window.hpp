#pragma once

#include "engine/internal_libs.hpp"
#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/vulkan/vk_instance.hpp"

#include <vulkan/vulkan.hpp>
#include <string>


namespace oe::graphics {

	class VKWindow : public Window {
	public:
		const VKInstance* m_instance;
		vk::SurfaceKHR m_surface;
		bool m_debugging;

	public:
		VKWindow(const VKInstance* instance, const WindowInfo& window_config);
		~VKWindow();

		void glfw();

		// Inherited via Window
		virtual void update() override;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) override;
		virtual void viewport() override;
		virtual void swapInterval(uint8_t frames) override;
	};

}