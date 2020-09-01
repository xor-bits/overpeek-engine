#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <gsl/span>
#include <gcem.hpp>



namespace oe {

	enum class formats {
		// all channels are one byte wide

		rgba, // red channel byte 0, green channel byte 2, blue channel byte 3, alpha channel byte 4, repeat
		rgb, // red channel byte 0, green channel byte 2, blue channel byte 3, repeat
		mono // white channel byte 0, repeat
	};

	constexpr size_t sizeofFormat(const formats& format)
	{
		switch (format)
		{
		case formats::rgba:
			return 4;
		case formats::rgb:
			return 3;
		case formats::mono:
			return 1;
		default:
			return 4;
		}
	}

	enum class modes {
		enable, 
		disable, 
		keep
	};
	enum class depth_functions {
		always, 
		never, 
		less_than,
		greater_than,
		equal,
		less_than_or_equal,
		greater_than_or_equal,
		disable
	};
	enum class culling_modes {
		neither, 
		both,
		front,
		back
	};
	enum class polygon_mode {
		fill, 
		lines, 
		points,
		none
	};

	// opengl GL_STATIC_.. and GL_DYNAMIC_..
	enum class types {
		none,
		static_type, 
		dynamic_type
	};

	// shader stages
	enum class shader_stages {
		vertex_shader, 
		tesselation_control_shader, 
		tesselation_evaluation_shader, 
		geometry_shader, 
		fragment_shader, 
		compute_shader
	};
	
	enum class primitive_types {
		points,
		lines,
		triangles, // independent triangles
		quads,     // independent quads
		custom,    // user defined
	};

	// supported graphics apis
	enum class graphics_api {
		OpenGL,
		Vulkan, // lacks proper implementation
		None    // can't use graphics
	};

	// gpu types
	enum class gpu {
		integrated,
		dedicated
	};

	// modifiers
	enum class modifiers {
		none = 0,
		shift = 0x0001,
		control = 0x0002,
		alt = 0x0004,
		super = 0x0008
	};

	// buttons
	enum class mouse_buttons {
		none = -1,
		button_last = 7,
		button_left = 0,
		button_right = 1,
		button_middle = 2
	};

	// actions
	enum class actions {
		none = -1,
		release = 0,
		press = 1,
		repeat = 2
	};

	// keys
	enum class keys {
		none = -1,
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
	enum class joystick {
		none = -1,
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
	enum class gamepad {
		none = -1,
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
		static constexpr glm::vec4 translucent_black = glm::vec4(0.0f, 0.0f, 0.0f, 0.25f);
		static constexpr glm::vec4 transparent = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		static constexpr glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		static constexpr glm::vec4 light_grey = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		static constexpr glm::vec4 grey = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		static constexpr glm::vec4 dark_grey = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		static constexpr glm::vec4 darker_grey = glm::vec4(0.125f, 0.125f, 0.125f, 1.0f);
		static constexpr glm::vec4 darkest_grey = glm::vec4(0.0625f, 0.0625f, 0.0625f, 1.0f);
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

		static constexpr glm::vec4 clear_color = glm::vec4(0.18f, 0.18f, 0.2f, 1.0f);

		static glm::vec4 rainbow(float t) { return glm::vec4(glm::vec4(sin(t), sin(t + (5.0f / 8.0f) * glm::pi<float>()), sin(t + (10.0f / 8.0f) * glm::pi<float>()), 1.0f)); };
		static glm::vec4 rainbow_bright(float t) { return (rainbow(t) + 1.0f) * 0.5f; };
	};

	struct alignments {
		static constexpr glm::vec2 top_left = glm::vec2(0.0f, 0.0f);
		static constexpr glm::vec2 center_left = glm::vec2(0.0f, 0.5f);
		static constexpr glm::vec2 bottom_left = glm::vec2(0.0f, 1.0f);
		static constexpr glm::vec2 top_center = glm::vec2(0.5f, 0.0f);
		static constexpr glm::vec2 center_center = glm::vec2(0.5f, 0.5f);
		static constexpr glm::vec2 bottom_center = glm::vec2(0.5f, 1.0f);
		static constexpr glm::vec2 top_right = glm::vec2(1.0f, 0.0f);
		static constexpr glm::vec2 center_right = glm::vec2(1.0f, 0.5f);
		static constexpr glm::vec2 bottom_right = glm::vec2(1.0f, 1.0f);
	};
	template<typename T = glm::vec2>
	T alignmentOffset(const T& size, const glm::vec2& alignment) {
		return static_cast<T>(static_cast<glm::vec2>(size) * alignment);
	}

	typedef std::function<void(oe::keys key, oe::actions action, oe::modifiers mods)> fun_key_callback;
	typedef std::function<void(oe::mouse_buttons button, oe::actions action)> fun_button_callback;
	typedef std::function<void(float delta)> fun_scroll_callback;
	typedef std::function<void(const glm::vec2 & framebuffer_size)> fun_resize_callback;
	typedef std::function<void(uint32_t codepoint, oe::modifiers mods)> fun_text_callback;
	typedef std::function<void(const glm::vec2 & transformed, const glm::vec2 & window)> fun_cursor_callback;
	typedef std::function<void(float)> fun_window_render;
	typedef std::function<void()> fun_window_update;

	// window open info
	struct WindowInfo {
		glm::ivec2 position = { 0, 0 };
		glm::ivec2 size = { 900, 600 };
		std::string title = "Overpeek Engine";
		unsigned char multisamples = 0;
		bool borderless = false;
		bool resizeable = true;
		bool transparent = false; // does nothing when using emscripten
		bool fullscreen = false;
		void* share_handle = nullptr; // pointer to the first window obj for multiwindow setups
		uint32_t swap_interval = 1;
		size_t main_updatesystem_ups = 60;
	};

	// renderer create info
	struct RendererInfo {
		int max_primitive_count = 10000;
		oe::types arrayRenderType = oe::types::dynamic_type;
		oe::types indexRenderType = oe::types::static_type;
		void* staticVBOBuffer_data = nullptr;
	};

	// shader per stage create info
	struct ShaderStageInfo {
		shader_stages stage;
		std::string source;
		std::string include_path; // optional
	};

	// shader create info, shader_stages is non-owning
	struct ShaderInfo {
		std::string name;
		std::vector<ShaderStageInfo> shader_stages;
	};

	// engine create info
	struct EngineInfo {
		bool audio = false;
		bool networking = false;
		graphics_api api = oe::graphics_api::OpenGL;
		gpu favored_gpu_vulkan = gpu::dedicated;
		bool debug_messages = false;
		bool ignore_errors = true;
	};

	struct TextureInfo {
		bool empty = false;
		const uint8_t* data;
		oe::formats data_format = oe::formats::rgba;
		bool generate_mipmaps = false;

		std::vector<size_t> size = { 1 };   // must have the same size as (std::vector<size_t> offset)
		std::vector<size_t> offset = { 0 }; // must have the same size as (std::vector<size_t> size)
	};

	struct FrameBufferInfo {
		glm::ivec2 size = { 1, 1 };
	};

	// events
    struct CursorPosEvent
    {
        glm::ivec2 cursor_windowspace = { 0.0f, 0.0f };
        glm::vec2 cursor_worldspace = { 0.0f, 0.0f };
    };

	struct ScrollEvent
    {
        glm::vec2 scroll_delta = { 0.0f, 0.0f };
    };

	struct MouseButtonEvent
	{
		CursorPosEvent cursor_pos;

        oe::mouse_buttons button = oe::mouse_buttons::none;
        oe::actions action = oe::actions::none;
        oe::modifiers mods = oe::modifiers::none;
	};

    struct KeyboardEvent
    {
        oe::keys key = oe::keys::none;
        oe::actions action = oe::actions::none;
        oe::modifiers mods = oe::modifiers::none;
    };

	struct CodepointEvent
	{
        // unicode text input
        char32_t codepoint = 0;
	};

	struct ResizeEvent
	{
        glm::ivec2 framebuffer_size = { 0, 0 };
        glm::ivec2 framebuffer_size_old = { 0, 0 };
		float aspect = 0.0f;
	};

#ifdef WIN32
#define __OE_FONT_NAME_R			"Arial.ttf"
#define __OE_FONT_NAME_B			"Arialbd.ttf"
#define __OE_FONT_NAME_I			"Ariali.ttf"
#define __OE_FONT_NAME_BI			"Arialbi.ttf"
#define __OE_FONT_PATH				"C:/Windows/Fonts/"
#elif __linux__
#define __OE_FONT_NAME_R			"LiberationSans-Regular.ttf"
#define __OE_FONT_NAME_B			"LiberationSans-Bold.ttf"
#define __OE_FONT_NAME_I			"LiberationSans-Italic.ttf"
#define __OE_FONT_NAME_BI			"LiberationSans-BoldItalic.ttf"
#define __OE_FONT_PATH				"/usr/share/fonts/truetype/liberation/"
#else
#define __OE_FONT_NAME_R			"LiberationSans-Regular.ttf"
#define __OE_FONT_NAME_B			"LiberationSans-Bold.ttf"
#define __OE_FONT_NAME_I			"LiberationSans-Italic.ttf"
#define __OE_FONT_NAME_BI			"LiberationSans-BoldItalic.ttf"
#define __OE_FONT_PATH				"/System/Library/Fonts"  /* I have no idea about fonts with MacOS */
#endif

#define __OE_FULL_FONT_PATH_R		__OE_FONT_PATH __OE_FONT_NAME_R
#define __OE_FULL_FONT_PATH_B		__OE_FONT_PATH __OE_FONT_NAME_B
#define __OE_FULL_FONT_PATH_I		__OE_FONT_PATH __OE_FONT_NAME_I
#define __OE_FULL_FONT_PATH_BI		__OE_FONT_PATH __OE_FONT_NAME_BI
#define __OE_FONT_NAME				__OE_FONT_NAME_R
#define __OE_FULL_FONT_PATH_R		__OE_FONT_PATH __OE_FONT_NAME_R
#define __OE_FULL_FONT_PATH_B		__OE_FONT_PATH __OE_FONT_NAME_B
#define __OE_FULL_FONT_PATH_I		__OE_FONT_PATH __OE_FONT_NAME_I
#define __OE_FULL_FONT_PATH_BI		__OE_FONT_PATH __OE_FONT_NAME_BI
#define __OE_FULL_FONT_PATH			__OE_FULL_FONT_PATH_R
	
	constexpr char default_font_name_regular[] = __OE_FONT_NAME_R;
	constexpr char default_font_name_bold[] = __OE_FONT_NAME_B;
	constexpr char default_font_name_italic[] = __OE_FONT_NAME_I;
	constexpr char default_font_name_bolditalic[] = __OE_FONT_NAME_BI;
	constexpr char default_font_name[] = __OE_FONT_NAME;

	constexpr char default_font_path[] = __OE_FONT_PATH;
	
	constexpr char default_full_font_path_regular[] = __OE_FULL_FONT_PATH_R;
	constexpr char default_full_font_path_bold[] = __OE_FULL_FONT_PATH_B;
	constexpr char default_full_font_path_italic[] = __OE_FULL_FONT_PATH_I;
	constexpr char default_full_font_path_bolditalic[] = __OE_FULL_FONT_PATH_BI;
	constexpr char default_full_font_path[] = __OE_FULL_FONT_PATH;

#undef __OE_FONT_NAME_R
#undef __OE_FONT_NAME_B
#undef __OE_FONT_NAME_I
#undef __OE_FONT_NAME_BI
#undef __OE_FONT_PATH

#undef __OE_FULL_FONT_PATH_R
#undef __OE_FULL_FONT_PATH_B
#undef __OE_FULL_FONT_PATH_I
#undef __OE_FULL_FONT_PATH_BI
#undef __OE_FONT_NAME
#undef __OE_FULL_FONT_PATH_R
#undef __OE_FULL_FONT_PATH_B
#undef __OE_FULL_FONT_PATH_I
#undef __OE_FULL_FONT_PATH_BI
#undef __OE_FULL_FONT_PATH
}