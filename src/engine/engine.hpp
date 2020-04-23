#pragma once

// Graphics
#include "graphics/interface/window.hpp"
#include "graphics/interface/shader.hpp"
#include "graphics/interface/renderer.hpp"
#include "graphics/interface/texture.hpp"
#include "graphics/interface/framebuffer.hpp"
#include "graphics/interface/instance.hpp"
#include "graphics/font.hpp"
#include "graphics/textLabel.hpp"

// Audio
#include "audio/audio.hpp"

// Logic
#include "utility/zip.hpp"
#include "utility/clock.hpp"
#include "utility/random.hpp"
#include "utility/gameloop.hpp"
#include "utility/fileio.hpp"
#include "utility/extra.hpp"

// Gui
#include "gui/gui_manager.hpp"

// internal libs
#include "internal_libs.hpp"

// Other
#include "graphics/sprite.hpp"
#include "graphics/spritePacker.hpp"
#include "networking/client.hpp"
#include "networking/server.hpp"

// Asset
#include "assets/default_shader/default_shader.hpp"

// #define OE_DEBUGGING
#ifdef OE_DEBUGGING
#define oe_debug_call(a) spdlog::debug("oe_debug_call: {}, file: {}", a, __FILE__)
#else
#define oe_debug_call(a)
#endif


namespace oe {

	class Engine {
	public:
		EngineInfo engine_info;

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
		void multipass(graphics::FrameBuffer& fb_0, graphics::FrameBuffer& fb_1, const graphics::Renderer& renderer, size_t count);

		graphics::Instance* createInstance(const InstanceInfo& instance_config);
		void destroyInstance(graphics::Instance* instance);

	
	};

}