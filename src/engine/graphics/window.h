#pragma once

#include <string>

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"


#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



class GLFWwindow;
namespace oe::graphics {

	class Window {
	public:
		typedef std::function<void(oe::keyboard key, oe::actions action, oe::modifiers mods)> key_callback;
		typedef std::function<void(oe::mouse button, oe::actions action)> button_callback;
		typedef std::function<void(float delta)> scroll_callback;
		typedef std::function<void(const glm::vec2 & framebuffer_size)> resize_callback;
		typedef std::function<void(uint32_t codepoint, oe::modifiers mods)> text_callback;
		typedef std::function<void(const glm::vec2 & transformed, const glm::vec2 & window)> cursor_callback;

	protected:
		WindowInfo m_window;
		GLFWwindow* m_window_handle = nullptr;

		bool m_keys[M_NUM_KEYS];
		bool m_buttons[M_NUM_BUTTONS];

		key_callback m_key_callback;
		button_callback m_button_callback;
		scroll_callback m_scroll_callback;
		resize_callback m_resize_callback;
		text_callback m_text_callback;
		cursor_callback m_cursor_callback;

		glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
		glm::vec2 m_cursor_window = { 0, 0 };
		float m_aspect_ratio = 0;

		void postglfw();

	public:
		Window(const WindowInfo& window_config);
		~Window();

		virtual void update() = 0;
		virtual void clear(const glm::vec4& color = glm::vec4(0.18f, 0.18f, 0.20f, 1.0f)) = 0;
		virtual void viewport() = 0;

		bool shouldClose();
		void setIcon(const oe::utils::image_data& image);
		void showCursor(bool show);

		void setKeyboardCallback(key_callback);
		void setButtonCallback(button_callback);
		void setScrollCallback(scroll_callback);
		void setResizeCallback(resize_callback);
		void setCharmodCallback(text_callback);
		void setCursorPositionCallback(cursor_callback);

	public:
		float aspect();
		float button(oe::mouse button);
		float key(oe::keyboard key);

		const glm::vec2& getPosition();
		void setPosition(const glm::vec2& pos);

		const glm::vec2& getSize();
		void setSize(const glm::vec2& size);

		const std::string& getTitle();
		void setTitle(const std::string& title);

		bool getBorderless();
		void setBorderless(bool borderless);

		bool getResizeable();
		void setResizeable(bool resizeable);

		bool getFullscreen();
		void setFullscreen(bool fullscreen);

		const glm::vec2& getCursorWindow();
		void setCursorWindow(const glm::vec2& cursor_at_window);

		const glm::vec2& getCursorTransformed();
		void setCursorTransformed(const glm::vec2& cursor_at_world_space);

		const std::string getClipboard();
		void setClipboard(const std::string& str);
	};

}