#pragma once

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
struct fmt::formatter<glm::vec3> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec3& d, FormatContext& ctx) {
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

namespace oe {

	static void init() {
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		spdlog::info("Engine initialized");
	}


#define error_terminate(error_msg, ...) __error(fmt::format(error_msg, __VA_ARGS__), __LINE__, __FILE__)

	static void terminate() {
#ifdef _DEBUG
		spdlog::info("Press <ENTER> to exit");
		assert(0);
#else
#endif // _DEBUG
	}

	static void __error(std::string error_msg, int line, std::string file) {
		spdlog::error("error: {}\nline: {}\nfile: {}", error_msg, line, file);
		oe::terminate();
	}

}