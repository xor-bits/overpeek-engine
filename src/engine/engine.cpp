#include "engine.h"



namespace oe {

	EngineInfo Engine::m_engine_info;

	void Engine::init(EngineInfo engine_info) {
		spdlog::set_pattern("%^[%T] [%l]:%$ %v");
		spdlog::set_level(spdlog::level::level_enum::trace);

		m_engine_info = engine_info;
		
		if (m_engine_info.audio) {
			spdlog::debug("Initializing audio");
			audio::Audio::init();
		}
		if (m_engine_info.networking) {
			spdlog::debug("Initializing networking");
			networking::enet::initEnet();
		}

		spdlog::debug("Engine initialized");
	}

	void Engine::deinit() {
		if (m_engine_info.audio) {
			spdlog::debug("Deinitializing audio");
			audio::Audio::deinit();
		}
		if (m_engine_info.networking) {
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

}