#include "engine.hpp"

#include "engine/graphics/interface/instance.hpp"
#include "engine/graphics/opengl/gl_instance.hpp"
#include "engine/graphics/opengl/gl_primitive_renderer.hpp"
#include "engine/utility/random.hpp"
#include "engine/utility/clock.hpp"
#include "engine/audio/audio.hpp"
#include "engine/networking/server.hpp"

#if defined(BUILD_VULKAN)
#include "engine/graphics/vulkan/vk_instance.hpp"
#include "engine/graphics/vulkan/vk_primitive_renderer.hpp"
#endif

#include <GLFW/glfw3.h>



static void glfw_error_func(int code, const char* desc) {
	spdlog::error("GLFW ({}): {}", code, desc);
}

namespace oe {

	Engine* Engine::singleton = nullptr;

	Engine::Engine() {
		// init to start the timer and seed the randomizer
		utils::Random::getSingleton().seed(utils::Clock::getSingleton().getMicroseconds());
	}

	void Engine::init(EngineInfo _engine_info) {
		oe_debug_call("engine");

		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		engine_info = _engine_info;

		glfwSetErrorCallback(glfw_error_func);
		if (!glfwInit()) oe_error_terminate("Failed to initialize GLFW!");
		
		if (engine_info.audio) {
			audio::Audio::init();
		}
		if (engine_info.networking) {
			networking::enet::initEnet();
		}

		srand(oe::utils::Clock::getSingleton().getMicroseconds());


		switch (engine_info.api)
		{
		case graphics_api::OpenGL:
			instance = new oe::graphics::GLInstance();
			break;
#ifdef BUILD_VULKAN
		case graphics_api::Vulkan:
			instance = new oe::graphics::VKInstance();
			break;
#endif
		default:
			instance = nullptr;
			break;
		}
	}

	void Engine::deinit() {
		if (engine_info.audio) {
			audio::Audio::deinit();
		}
		if (engine_info.networking) {
			networking::enet::deinitEnet();
		}
		if (instance) delete instance;
	}

	void Engine::terminate() {
#ifdef _DEBUG
		assert(0);
#else
#endif // _DEBUG
	}

	void Engine::__error(std::string error_msg, int line, std::string file) {
		spdlog::error("error: {}\nline: {}\nfile: {}", error_msg, line, file);
		oe::Engine::terminate();
	}

	void* Engine::createPrimitiveRenderer(const RendererInfo& renderer_info)
	{
		switch (engine_info.api)
		{
		case graphics_api::OpenGL:
			return new oe::graphics::GLPrimitiveRenderer(renderer_info);
#ifdef BUILD_VULKAN
		case graphics_api::Vulkan:
			return new oe::graphics::VKPrimitiveRenderer(renderer_info);
#endif
		default:
			return nullptr;
		}
	}

	void Engine::destroyPrimitiveRenderer(void* renderer)
	{
		delete (graphics::PrimitiveRenderer*)renderer;
	}


}