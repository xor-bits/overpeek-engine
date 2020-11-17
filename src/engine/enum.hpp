#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <gsl/span>
#include <gcem.hpp>

#include "engine/utility/font_file.hpp"



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
		keep,
	};
	enum class depth_functions {
		disable,
		always, 
		never, 
		less_than,
		greater_than,
		equal,
		less_than_or_equal,
		greater_than_or_equal,
	};
	enum class culling_modes {
		neither, 
		both,
		front,
		back,
	};
	enum class polygon_mode {
		none,
		fill, 
		lines, 
		points,
	};

	// opengl GL_STATIC_.. and GL_DYNAMIC_..
	enum class types {
		none,
		static_type, 
		dynamic_type,
	};

	// shader stages
	enum class shader_stages {
		none,
		vertex_shader, 
		tesselation_control_shader, 
		tesselation_evaluation_shader, 
		geometry_shader, 
		fragment_shader, 
		compute_shader,
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
		none,   // can't use graphics
		OpenGL, // should be used
		Vulkan, // lacks proper implementation
	};

	// gpu types
	enum class gpu {
		integrated,
		dedicated,
	};

	// modifiers
	enum class modifiers {
		none = 0,
		shift = 1<<0,
		control = 1<<1,
		alt = 1<<2,
		super = 1<<3,
	};

	[[nodiscard]] constexpr inline modifiers operator|(const modifiers a, const modifiers b)
	{
		return static_cast<modifiers>(static_cast<int>(a) | static_cast<int>(b));
	}

	[[nodiscard]] constexpr inline modifiers operator&(const modifiers a, const modifiers b)
	{
		return static_cast<modifiers>(static_cast<int>(a) & static_cast<int>(b));
	}

	// buttons
	enum class mouse_buttons {
		none = -1,
		button_last = 7,
		button_left = 0,
		button_right = 1,
		button_middle = 2,
	};

	// actions
	enum class actions {
		none = -1,
		release = 0,
		press = 1,
		repeat = 2,
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
		key_menu = 348,
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
		joystick_last         = joystick_16,
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
		gamepad_triangle = gamepad_y,
	};
	
	enum class interact_type_flags
	{
		none = 0<<0,
		cursor = 1<<0,
		scroll = 1<<1,
		keyboard = 1<<2,
	};

	[[nodiscard]] constexpr inline interact_type_flags operator|(const interact_type_flags a, const interact_type_flags b)
	{
		return static_cast<interact_type_flags>(static_cast<int>(a) | static_cast<int>(b));
	}

	[[nodiscard]] constexpr inline interact_type_flags operator&(const interact_type_flags a, const interact_type_flags b)
	{
		return static_cast<interact_type_flags>(static_cast<int>(a) & static_cast<int>(b));
	}

	enum class texture_wrap {
		repeat,
		mirrored_repeat,
		clamp_to_edge,
		clamp_to_border,
	};

	enum class texture_filter {
		nearest,
		linear,
	};

	using color = glm::vec<4, float, glm::packed_highp>;
	// some predefined colors
	struct colors
	{
		static constexpr oe::color translucent_black = { 0.0f, 0.0f, 0.0f, 0.25f };
		static constexpr oe::color transparent       = { 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr oe::color white             = { 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr oe::color light_grey        = { 0.75f, 0.75f, 0.75f, 1.0f };
		static constexpr oe::color grey              = { 0.5f, 0.5f, 0.5f, 1.0f };
		static constexpr oe::color dark_grey         = { 0.25f, 0.25f, 0.25f, 1.0f };
		static constexpr oe::color darker_grey       = { 0.125f, 0.125f, 0.125f, 1.0f };
		static constexpr oe::color darkest_grey      = { 0.0625f, 0.0625f, 0.0625f, 1.0f };
		static constexpr oe::color black             = { 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr oe::color red               = { 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr oe::color green             = { 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr oe::color blue              = { 0.0f, 0.0f, 1.0f, 1.0f };
		static constexpr oe::color dark_red          = { 0.5f, 0.0f, 0.0f, 1.0f };
		static constexpr oe::color dark_green        = { 0.0f, 0.5f, 0.0f, 1.0f };
		static constexpr oe::color dark_blue         = { 0.0f, 0.0f, 0.5f, 1.0f };
		static constexpr oe::color light_red         = { 1.0f, 0.5f, 0.5f, 1.0f };
		static constexpr oe::color light_green       = { 0.5f, 1.0f, 0.5f, 1.0f };
		static constexpr oe::color light_blue        = { 0.5f, 0.5f, 1.0f, 1.0f };
		static constexpr oe::color cyan              = { 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr oe::color magenta           = { 1.0f, 0.0f, 1.0f, 1.0f };
		static constexpr oe::color yellow            = { 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr oe::color orange            = { 1.0f, 0.5f, 0.0f, 1.0f };
		static constexpr oe::color pink              = { 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr oe::color lime              = { 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr oe::color mint              = { 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr oe::color purple            = { 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr oe::color sky               = { 0.0f, 0.5f, 1.0f, 1.0f };

		static constexpr oe::color clear_color       = { 0.18f, 0.18f, 0.2f, 1.0f };

		static constexpr oe::color rainbow(float t) { return oe::color(gcem::sin(t), gcem::sin(t + (5.0f / 8.0f) * glm::pi<float>()), gcem::sin(t + (10.0f / 8.0f) * glm::pi<float>()), 1.0f); };
		static constexpr oe::color rainbow_bright(float t) { return (rainbow(t) + 1.0f) * 0.5f; };
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
	template<typename T = float>
	glm::vec<2, T> alignmentOffset(const glm::vec<2, T>& size, const glm::vec2& alignment)
	{
		return static_cast<glm::vec<2, T>>((static_cast<glm::vec2>(size) * alignment));
	}
	template<typename T = float>
	glm::vec<2, T> alignmentOffsetRound(const glm::vec<2, T>& size, const glm::vec2& alignment)
	{
		return static_cast<glm::vec<2, T>>(glm::round(static_cast<glm::vec2>(size) * alignment));
	}

	// window open info
	struct WindowInfo
	{
		glm::ivec2 position = { 0, 0 };
		glm::uvec2 size = { 900, 600 };
		std::string title = "Overpeek Engine";
		uint8_t multisamples = 0;
		bool borderless = false;
		bool resizeable = true;
		bool transparent = false; // does nothing when using emscripten
		bool fullscreen = false;
		void* share_handle = nullptr; // pointer to the first window obj for multiwindow setups
		uint8_t swap_interval = 1;
		size_t main_updatesystem_ups = 60;
	};

	// renderer create info
	struct RendererInfo
	{
		int32_t max_primitive_count = 10000;
		oe::types arrayRenderType = oe::types::dynamic_type;
		oe::types indexRenderType = oe::types::static_type;
		void* staticVBOBuffer_data = nullptr;
	};

	struct PolygonRendererInfo
	{
		int32_t vertex_count = 10000;
		oe::types vertex_storage_type = oe::types::dynamic_type;
		int32_t index_count  = 10000;
		oe::types index_storage_type  = oe::types::dynamic_type;

		void* static_vertex_data = nullptr;
		void* static_index_data  = nullptr;
	};

	// shader per stage create info
	struct ShaderStageInfo
	{
		shader_stages stage;
		std::string_view source;
		std::string_view include_path; // optional
	};

	// shader create info, shader_stages is non-owning
	struct ShaderInfo
	{
		std::string name;
		std::vector<ShaderStageInfo> shader_stages;
#ifdef OE_BUILD_MODE_SHADERC
		bool shaderc_optimize = true;
#endif // OE_BUILD_MODE_SHADERC
	};

	// engine create info
	struct EngineInfo
	{
		bool networking = false;
		bool networking_init_noexcept = true;

		graphics_api api = oe::graphics_api::OpenGL;
		gpu favored_gpu_vulkan = gpu::dedicated;
		bool debug_mode = true;
		bool ignore_errors = true;
		std::string asset_path = "asset.zip";
	};

	struct TextureInfo
	{
		bool empty = false;
		const uint8_t* data;
		oe::formats data_format = oe::formats::rgba;
		bool generate_mipmaps = false;

		texture_wrap wrap = texture_wrap::clamp_to_border;
		texture_filter filter = texture_filter::nearest;

		using size_offset_pair_t = std::pair<int32_t, int32_t>;
		std::vector<size_offset_pair_t> size_offset = { { 1, 0 } };
	};

	struct FrameBufferInfo
	{
		glm::uvec2 size = { 1, 1 };
	};

	struct TextOptions
	{
		bool                 enabled = true;
		oe::utils::FontFile     font = {};
		glm::vec2              align = oe::alignments::center_center;
		oe::color initial_text_color = oe::colors::white;
		oe::color   background_color = oe::colors::transparent;
		uint16_t                size = 16;
		float                 weight = 0.25f;
		oe::color      outline_color = oe::colors::black;
		float         outline_weight = 0.3f;
		float             anti_alias = 0.2f;
	};

	// events
	template<size_t ups> struct UPS { static constexpr size_t system_ups = ups; };
	template<size_t ups> using UpdateEvent = UPS<ups>;
	struct InitEvent {};
	struct CleanupEvent {};
	struct RenderEvent {};

    struct CursorPosEvent
    {
        glm::ivec2 cursor_windowspace = { 0, 0 };
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
        oe::modifiers mods = oe::modifiers::none;
	};

	struct ResizeEvent
	{
        glm::uvec2 framebuffer_size = { 0, 0 };
        glm::uvec2 framebuffer_size_old = { 0, 0 };
		float aspect = 0.0f;
	};
}