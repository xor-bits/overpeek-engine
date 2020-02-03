#pragma once

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MINIZ_NO_ZLIB_COMPATIBLE_NAMES
#include <miniz.h>

#include <entt/entt.hpp>



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
#define quickDebug(x) spdlog::debug(#x ## ": {}", x)
#define oe_error_terminate(error_msg, ...) oe::__error(fmt::format(error_msg, ##__VA_ARGS__), __LINE__, __FILE__)
#define oe_terminate() oe::terminate()
namespace oe {

	int argc();
	char** argv();

	void init(int argc, char* argv[]);

	void terminate();
	void __error(std::string error_msg, int line, std::string file);

	struct colors {
	public:
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
