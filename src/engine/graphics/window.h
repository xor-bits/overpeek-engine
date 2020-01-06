#pragma once

#include <string>

#include "engine/utility/filereader.h"
#include "engine/internal_libs.h"

#define WINDOW_MULTISAMPLE_X2	0b0000000000000001
#define WINDOW_MULTISAMPLE_X4	0b0000000000000010
#define WINDOW_MULTISAMPLE_X8	0b0000000000000100
#define WINDOW_BORDERLESS		0b0000000000001000
#define WINDOW_RESIZEABLE		0b0000000000010000
#define WINDOW_TRANSPARENT		0b0000000000100000
#define WINDOW_FULLSCREEN		0b0000000001000000
#define WINDOW_GL_DEBUG			0b0000000010000000



namespace oe::graphics {
	
	class Window {
	private:
		//Variables
		static int p_width, p_height;
		static void* p_window;
		static glm::vec2 p_mouse;

		static bool p_keys[];
		static bool p_buttons[];

		static bool p_debugmode;

		static void(*mKeyCallback)(int, int);
		static void(*mButtonCallback)(int, int);
		static void(*m_scroll_callback)(double);
		static void(*m_resize_callback)(int, int);
		static void(*m_charmods_callback)(unsigned int, int);

		//Functions
		static void mouse_button_callback(int button, int action, int mods);
		static void key_callback(int key, int scancode, int action, int mods);
		static void cursor_position_callback(double xpos, double ypos);
		static void scroll_callback(double xoffset, double yoffset);
		static void framebuffer_size_callback(int numer, int denom);
		static void charmods_callback(unsigned int codepoint, int mods);
		
	public:
		/*
		mods	-	window modification flags prefixed with WINDOW_
		*/
		static int init(unsigned int width, unsigned int height, std::string title, int mods = NULL);

		static bool shouldClose();
		static void input();
		static void update();
		static void clear();
		static void close();

		static void setButtonCallback(void(*callback)(int, int));
		static void setKeyboardCallback(void(*callback)(int, int));
		static void setScrollCallback(void(*callback)(double));
		static void setResizeCallback(void(*callback)(int, int));
		static void setCharmodCallback(void(*callback)(unsigned int, int));

		static void viewport();

		static void setClearColor(float r, float g, float b, float a);

		static void setIcon(const oe::utils::image_data& image);

		static void showCursor(bool show);
		static void setCursorPos(double x, double y);

		static inline bool getButton(int button) { return p_buttons[button]; }
		static inline bool getKey(int key) { return p_keys[key]; }
		static inline glm::vec2& getMousePos() { return p_mouse; }

		static inline float getAspect() { return (float)getWidth() / (float)getHeight(); }
		static inline int getHeight() { return p_height; }
		static inline int getWidth() { return p_width; }
	};

}