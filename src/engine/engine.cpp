#include "engine.hpp"

#include "graphics/opengl/gl_instance.hpp"
#include "graphics/vulkan/vk_instance.hpp"

#include <GLFW/glfw3.h>



static void glfw_error_func(int code, const char* desc) {
	spdlog::error("GLFW ({}): {}", code, desc);
}

namespace oe {

	EngineInfo Engine::engine_info;

	void Engine::init(EngineInfo _engine_info) {
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
	}

	void Engine::deinit() {
		if (engine_info.audio) {
			audio::Audio::deinit();
		}
		if (engine_info.networking) {
			networking::enet::deinitEnet();
		}
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

	graphics::Instance* Engine::createInstance(const InstanceInfo& instance_config) {
		if (engine_info.api == oe::graphics_api::Vulkan) {
			return new oe::graphics::VKInstance(instance_config);
		}
		else {
			return new oe::graphics::GLInstance(instance_config);
		}
	}

	void Engine::destroyInstance(graphics::Instance* instance) {
		delete instance;
	}

}