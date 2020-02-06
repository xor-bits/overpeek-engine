#include "engine.hpp"

#include "graphics/opengl/gl_instance.hpp"



namespace oe {

	EngineInfo Engine::engine_info;

	void Engine::init(EngineInfo _engine_info) {
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		engine_info = _engine_info;
		
		if (engine_info.audio) {
			spdlog::debug("Initializing audio");
			audio::Audio::init();
		}
		if (engine_info.networking) {
			spdlog::debug("Initializing networking");
			networking::enet::initEnet();
		}

		spdlog::debug("Engine initialized");
	}

	void Engine::deinit() {
		if (engine_info.audio) {
			spdlog::debug("Deinitializing audio");
			audio::Audio::deinit();
		}
		if (engine_info.networking) {
			spdlog::debug("Deinitializing networking");
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
			// Vulkan instance
		}
		else {
			return new oe::graphics::GLInstance(instance_config);
		}
	}

	void Engine::destroyInstance(graphics::Instance* instance) {
		delete instance;
	}

}