#pragma once

#include "engine/enum.hpp"



namespace oe::graphics { class Instance; }
namespace oe {

	class Engine {
	public:
		EngineInfo engine_info;
		oe::graphics::Instance* instance;
		depth_functions current_depth;
		static size_t draw_calls;

	private:
		static Engine* singleton;
		Engine();

	public:
		Engine(const Engine&) = delete;
		static Engine& getSingleton() {
			if (!singleton) singleton = new Engine();
			return *singleton;
		}

		void init(EngineInfo engine_info);
		void deinit();

		void terminate();
		void __error(std::string error_msg, int line, std::string file);

		// set blending mode
		void blending(oe::modes mode = oe::modes::enable) const;

		// set depth function or disable it
		void depth(depth_functions func = depth_functions::always);
		depth_functions getDepth() const;

		// also known as vertical sync
		void swapInterval(unsigned int interval = 0) const;

		// stop drawing any side of primitives
		void culling(culling_modes c = culling_modes::neither) const;

		// width of all lines in pixels
		void lineWidth(float w = 1.0f) const;

		// radius of all points in pixels
		void pointRadius(float w = 1.0f) const;

		// primitive draw mode
		void polygonMode(polygon_mode p = polygon_mode::fill) const;
	
	};
}



template <>
struct fmt::formatter<glm::vec2> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec2& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {} ]", d.x, d.y);
	}
};

template <>
struct fmt::formatter<glm::ivec2> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::ivec2& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {} ]", d.x, d.y);
	}
};

template <>
struct fmt::formatter<glm::vec3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec3& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {} ]", d.x, d.y, d.z);
	}
};

template <>
struct fmt::formatter<glm::ivec3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::ivec3& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {} ]", d.x, d.y, d.z);
	}
};

template <>
struct fmt::formatter<glm::vec4> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec4& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {}, {} ]", d.x, d.y, d.z, d.w);
	}
};

template <>
struct fmt::formatter<glm::ivec4> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::ivec4& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {}, {} ]", d.x, d.y, d.z, d.w);
	}
};

template <>
struct fmt::formatter<glm::mat4> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::mat4& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {}, {} ]", glm::vec4(d[0][0], d[0][1], d[0][2], d[0][3]), glm::vec4(d[1][0], d[1][1], d[1][2], d[1][3]), glm::vec4(d[2][0], d[2][1], d[2][2], d[2][3]), glm::vec4(d[3][0], d[3][1], d[3][2], d[3][3]));
	}
};

template <>
struct fmt::formatter<glm::mat3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::mat3& d, FormatContext& ctx) {
		return format_to(ctx.out(), "[ {}, {}, {} ]", glm::vec3(d[0][0], d[0][1], d[0][2]), glm::vec3(d[1][0], d[1][1], d[1][2]), glm::vec3(d[2][0], d[2][1], d[2][2]));
	}
};

// (input) -> ("input: {} line file", input)
#define quickDebug(x) spdlog::debug(#x ## ": {} at line {} in file {}", x, __LINE__, __FILE__)

#define oe_error_terminate(error_msg, ...) oe::Engine::getSingleton().__error(fmt::format(error_msg, ##__VA_ARGS__), __LINE__, __FILE__)
#define oe_terminate() oe::Engine::getSingleton().terminate()

// #define OE_DEBUGGING
#ifdef OE_DEBUGGING
#define oe_debug_call(a) spdlog::debug("oe_debug_call: {}, file: {}", a, __FILE__)
#else
#define oe_debug_call(a)
#endif