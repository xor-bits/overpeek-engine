#include "engine.hpp"

#include "engine/graphics/interface/instance.hpp"
#include "engine/utility/random.hpp"
#include "engine/utility/clock.hpp"
#include "engine/audio/audio.hpp"
#include "engine/networking/server.hpp"


#include "engine/graphics/opengl/gl_instance.hpp"
#if defined(BUILD_VULKAN)
#include "engine/graphics/vulkan/vk_instance.hpp"
#endif

#include <GLFW/glfw3.h>



static void glfw_error_func(int code, const char* desc) {
	spdlog::error("GLFW ({}): {}", code, desc);
}

namespace oe {

	Engine* Engine::singleton = nullptr;
	size_t Engine::draw_calls = 0;

	Engine::Engine()
	{
		// init to start the timer and seed the randomizer
		utils::Random::getSingleton().seed(utils::Clock::getSingleton().getMicroseconds());
	}

	void Engine::init(const EngineInfo& _engine_info)
	{
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		engine_info = _engine_info;

		glfwSetErrorCallback(glfw_error_func);
		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW!");
		
		if (engine_info.networking)
			try{
				networking::enet::initEnet();
			}catch(const std::exception& e){
				spdlog::warn(e.what());
				if(!engine_info.networking_init_noexcept)
					throw e;
			}

		switch (engine_info.api)
		{
		case graphics_api::OpenGL:
			instance = std::make_unique<oe::graphics::GLInstance>();
			break;
#ifdef BUILD_VULKAN
		case graphics_api::Vulkan:
			instance = std::make_unique<oe::graphics::VKInstance>();
			break;
#endif
		default:
			instance = nullptr;
			break;
		}

#ifdef OE_BUILD_MODE_SHADERC
		spdlog::debug("Using Shaderc for glsl optimization / spir-v compilation");
#endif // OE_BUILD_MODE_SHADERC

	}

	void Engine::deinit()
	{
		if (engine_info.networking)
			networking::enet::deinitEnet();
		instance.reset();
	}

	void Engine::terminate()
	{
		assert(0);
	}

	void Engine::__error(const std::string& error_msg, int line, const std::string& file)
	{
		std::string error_str = fmt::format("error: {}\nline: {}\nfile: {}", error_msg, line, file);
#ifdef OE_TERMINATE_IS_THROW
		throw std::runtime_error(error_str);
#else
		spdlog::error("{}", error_str);

		if (!engine_info.ignore_errors)
			oe::Engine::terminate();
#endif
	}

	void Engine::setRasterizerInfo(const RasterizerInfo& i)
	{
		if(rasterizer_info.blend_mode != i.blend_mode)
			instance->blending(i.blend_mode);
		if(rasterizer_info.cull_face != i.cull_face)
			instance->culling(i.cull_face);
		if(rasterizer_info.depth_func != i.depth_func)
			instance->depth(i.depth_func);
		if(rasterizer_info.fill_mode != i.fill_mode)
			instance->polygonMode(i.fill_mode);
		if(rasterizer_info.line_width != i.line_width)
			instance->lineWidth(i.line_width);
		if(rasterizer_info.point_radius != i.point_radius)
			instance->pointRadius(i.point_radius);

		rasterizer_info = i;
	}
}