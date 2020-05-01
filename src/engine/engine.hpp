#pragma once

#include "engine/enum.hpp"
#include "engine/graphics/interface/instance.hpp"



namespace oe::graphics { class Window; class Shader; class Texture; class FrameBuffer; class Renderer; }
namespace oe {

	class Engine {
	public:
		EngineInfo engine_info;
		oe::graphics::Instance* instance;

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

		graphics::Window* createWindow(const WindowInfo& window_config) const { return instance->createWindow(window_config); }
		graphics::Shader* createShader(const ShaderInfo& shader_info) const { return instance->createShader(shader_info); }
		graphics::Texture* createTexture(const TextureInfo& texture_info) const { return instance->createTexture(texture_info); }
		graphics::FrameBuffer* createFrameBuffer(const FrameBufferInfo& framebuffer_info, graphics::Window* window) const { return instance->createFrameBuffer(framebuffer_info, window); }
		graphics::Renderer* createRenderer(const RendererInfo& renderer_info) { return instance->createRenderer(renderer_info); }

		void destroyWindow(graphics::Window* window) const { instance->destroyWindow(window); }
		void destroyShader(graphics::Shader* shader) const { instance->destroyShader(shader); }
		void destroyTexture(graphics::Texture* texture) const { instance->destroyTexture(texture); }
		void destroyFrameBuffer(graphics::FrameBuffer* framebuffer) const { instance->destroyFrameBuffer(framebuffer); }
		void destroyRenderer(graphics::Renderer* renderer) const { instance->destroyRenderer(renderer); }

		// set blending mode
		void blending(oe::modes mode = oe::modes::enable) const { instance->blending(mode); }

		// set depth function or disable it
		void depth(depth_functions func = depth_functions::always) const { instance->depth(func); }

		// also known as vertical sync
		void swapInterval(unsigned int interval = 0) const { instance->swapInterval(interval); }

		// stop drawing any side of primitives
		void culling(culling_modes c = culling_modes::neither) const { instance->culling(c); }

		// width of all lines in pixels
		void lineWidth(float w = 1.0f) const { instance->lineWidth(w); }

		// radius of all points in pixels
		void pointRadius(float w = 1.0f) const { instance->pointRadius(w); }

		// primitive draw mode
		void polygonMode(polygon_mode p = polygon_mode::fill) const { instance->polygonMode(p); }
	
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