#pragma once

#include <glm/glm.hpp>



namespace oe {

	// supported graphics apis
	enum graphics_api {
		OpenGL, Vulkan
	};

	struct EngineInfo {
		bool audio = false;
		bool networking = false;
		graphics_api api = Vulkan;
	};

	// modifiers
	enum modifiers {
		shift = 0x0001,
		control = 0x0002,
		alt = 0x0004,
		super = 0x0008
	};

	// buttons
	enum mouse {
		button_last = 7,
		button_left = 0,
		button_right = 1,
		button_middle = 2
	};

	// actions
	enum actions {
		release = 0,
		press = 1,
		repeat = 2
	};

	// keys
	enum keyboard {
		key_space = 32,
		key_apostrophe = 39,     /* ' */
		key_comma = 44,		   /* , */
		key_minus = 45,		   /* - */
		key_period = 46,	       /* . */
		key_slash = 47,		   /* / */
		key_0 = 48,
		key_1 = 49,
		key_2 = 50,
		key_3 = 51,
		key_4 = 52,
		key_5 = 53,
		key_6 = 54,
		key_7 = 55,
		key_8 = 56,
		key_9 = 57,
		key_semicolon = 59,      /* ; */
		key_equal = 61,		   /* = */
		key_a = 65,
		key_b = 66,
		key_c = 67,
		key_d = 68,
		key_e = 69,
		key_f = 70,
		key_g = 71,
		key_h = 72,
		key_i = 73,
		key_j = 74,
		key_k = 75,
		key_l = 76,
		key_m = 77,
		key_n = 78,
		key_o = 79,
		key_p = 80,
		key_q = 81,
		key_r = 82,
		key_s = 83,
		key_t = 84,
		key_u = 85,
		key_v = 86,
		key_w = 87,
		key_x = 88,
		key_y = 89,
		key_z = 90,
		key_left_bracket = 91,   /* [ */
		key_backslash = 92,	   /* \ */
		key_right_bracket = 93,  /* ] */
		key_grave_accent = 96,   /* ` */
		key_world_1 = 161,	   /* non-us #1 */
		key_world_2 = 162,	   /* non-us #2 */

		key_escape = 256,
		key_enter = 257,
		key_tab = 258,
		key_backspace = 259,
		key_insert = 260,
		key_delete = 261, // "delete" is keyword in c++
		key_right = 262,
		key_left = 263,
		key_down = 264,
		key_up = 265,
		key_page_up = 266,
		key_page_down = 267,
		key_home = 268,
		key_end = 269,
		key_caps_lock = 280,
		key_scroll_lock = 281,
		key_num_lock = 282,
		key_print_screen = 283,
		key_pause = 284,
		key_f1 = 290,
		key_f2 = 291,
		key_f3 = 292,
		key_f4 = 293,
		key_f5 = 294,
		key_f6 = 295,
		key_f7 = 296,
		key_f8 = 297,
		key_f9 = 298,
		key_f10 = 299,
		key_f11 = 300,
		key_f12 = 301,
		key_f13 = 302,
		key_f14 = 303,
		key_f15 = 304,
		key_f16 = 305,
		key_f17 = 306,
		key_f18 = 307,
		key_f19 = 308,
		key_f20 = 309,
		key_f21 = 310,
		key_f22 = 311,
		key_f23 = 312,
		key_f24 = 313,
		key_f25 = 314,
		key_numpad_0 = 320,
		key_numpad_1 = 321,
		key_numpad_2 = 322,
		key_numpad_3 = 323,
		key_numpad_4 = 324,
		key_numpad_5 = 325,
		key_numpad_6 = 326,
		key_numpad_7 = 327,
		key_numpad_8 = 328,
		key_numpad_9 = 329,
		key_numpad_decimal = 330,
		key_numpad_divide = 331,
		key_numpad_multiply = 332,
		key_numpad_subtract = 333,
		key_numpad_add = 334,
		key_numpad_enter = 335,
		key_numpad_equal = 336,
		key_left_shift = 340,
		key_left_control = 341,
		key_left_alt = 342,
		key_left_super = 343,
		key_right_shift = 344,
		key_right_control = 345,
		key_right_alt = 346,
		key_right_super = 347,
		key_menu = 348
	};

	// joystick input
	enum joystick {
		joystick_1            = 0 ,
		joystick_2            = 1 ,
		joystick_3            = 2 ,
		joystick_4            = 3 ,
		joystick_5            = 4 ,
		joystick_6            = 5 ,
		joystick_7            = 6 ,
		joystick_8            = 7 ,
		joystick_9            = 8 ,
		joystick_10           = 9 ,
		joystick_11           = 10,
		joystick_12           = 11,
		joystick_13           = 12,
		joystick_14           = 13,
		joystick_15           = 14,
		joystick_16           = 15,
		joystick_last         = joystick_16
	};

	// gamepad input
	enum gamepad {
		gamepad_a = 0,
		gamepad_b = 1,
		gamepad_x = 2,
		gamepad_y = 3,
		gamepad_left_bumper = 4,
		gamepad_right_bumper = 5,
		gamepad_back = 6,
		gamepad_start = 7,
		gamepad_guide = 8,
		gamepad_left_thumb = 9,
		gamepad_right_thumb = 10,
		gamepad_dpad_up = 11,
		gamepad_dpad_right = 12,
		gamepad_dpad_down = 13,
		gamepad_dpad_left = 14,
		gamepad_last = gamepad_dpad_left,

		gamepad_cross = gamepad_a,
		gamepad_circle = gamepad_b,
		gamepad_square = gamepad_x,
		gamepad_triangle = gamepad_y
	};

	// some predefined colors
	struct colors {
		static constexpr glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 grey = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		static constexpr glm::vec4 black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 dark_red = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 dark_green = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
		static constexpr glm::vec4 dark_blue = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		static constexpr glm::vec4 light_red = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
		static constexpr glm::vec4 light_green = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
		static constexpr glm::vec4 light_blue = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
		static constexpr glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 magenta = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 yellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 orange = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
		static constexpr glm::vec4 pink = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
		static constexpr glm::vec4 lime = glm::vec4(0.5f, 1.0f, 0.0f, 1.0f);
		static constexpr glm::vec4 mint = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);
		static constexpr glm::vec4 purple = glm::vec4(0.5f, 0.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 sky = glm::vec4(0.0f, 0.5f, 1.0f, 1.0f);
	};

}