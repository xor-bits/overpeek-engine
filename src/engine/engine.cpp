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

		if (!oe::Engine::getSingleton().engine_info.ignore_errors)
			oe::Engine::terminate();
	}


	void Engine::blending(oe::modes mode) const { instance->blending(mode); }
	void Engine::depth(depth_functions func) { current_depth = func; instance->depth(func); }
	depth_functions Engine::getDepth() const { return current_depth; }
	void Engine::swapInterval(unsigned int interval) const { instance->swapInterval(interval); }
	void Engine::culling(culling_modes c) const { instance->culling(c); }
	void Engine::lineWidth(float w) const { instance->lineWidth(w); }
	void Engine::pointRadius(float w) const { instance->pointRadius(w); }
	void Engine::polygonMode(polygon_mode p) const { instance->polygonMode(p); }
}