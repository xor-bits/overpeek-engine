#pragma once

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include <miniz.h>

#include <entt/entt.hpp>

#include "enum.h"



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

// (input) -> ("input: {}", input)
#define quickDebug(x) spdlog::debug(#x ## ": {} at line {} in file {}", x, __LINE__, __FILE__)

#define oe_error_terminate(error_msg, ...) oe::Engine::__error(fmt::format(error_msg, ##__VA_ARGS__), __LINE__, __FILE__)
#define oe_terminate() oe::Engine::terminate()