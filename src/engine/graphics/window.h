#pragma once

#include <string>

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"



namespace oe::graphics {
	
	class Window {
	public:
		struct WindowConfig {
			glm::vec2 position = { 0, 0 };
			glm::vec2 size = { 900, 600 };
			std::string title = "Overpeek Engine";
			unsigned char multisamples = 0;
			bool borderless = false;
			bool resizeable = true;
			bool transparent = false;
			bool fullscreen = false;

			bool opengl_debugmode = false;
		};
		
	public:
		static void init(WindowConfig& window_config);

		static bool windowShouldClose();
		
		static void pollEvents();
		static void updateWindow();
		static void clearWindow(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f));
		static void closeWindow();

		static void viewport();
		static void setIcon(const oe::utils::image_data& image);
		static void showCursor(bool show);

		static void setKeyboardCallback(void(*callback)(int key, int action, int mods));
		static void setButtonCallback(void(*callback)(int button, int action));
		static void setScrollCallback(void(*callback)(double delta));
		static void setResizeCallback(void(*callback)(const glm::vec2& framebuffer_size));
		static void setCharmodCallback(void(*callback)(unsigned int codepoint, int mods));
		static void setCursorPositionCallback(void(*callback)(const glm::vec2& transformed, const glm::vec2& window));

	public:
		static float aspect();
		static float button(int button);
		static float key(int key);

		static const glm::vec2& getPosition();
		static void setPosition(const glm::vec2& pos);

		static const glm::vec2& getSize();
		static void setSize(const glm::vec2& size);

		static const std::string& getTitle();
		static void setTitle(const std::string& title);

		static bool getBorderless();
		static void setBorderless(bool borderless);

		static bool getResizeable();
		static void setResizeable(bool resizeable);

		static bool getFullscreen();
		static void setFullscreen(bool fullscreen);

		static const glm::vec2& getCursorWindow();
		static void setCursorWindow(const glm::vec2& cursor_at_window);

		static const glm::vec2& getCursorTransformed();
		static void setCursorTransformed(const glm::vec2& cursor_at_world_space);
	};

}