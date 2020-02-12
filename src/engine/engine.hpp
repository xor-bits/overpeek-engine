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
#include "utility/math.hpp"
#include "utility/clock.hpp"
#include "utility/debug.hpp"
#include "utility/random.hpp"
#include "utility/gameloop.hpp"
#include "utility/filereader.hpp"

// Gui
#include "gui/gui_manager.hpp"

// internal libs
#include "internal_libs.hpp"

// Other
#include "graphics/sprite.hpp"
#include "graphics/spritePacker.hpp"
#include "networking/client.hpp"
#include "networking/server.hpp"



namespace oe {

	class Engine {
	public:
		static EngineInfo engine_info;

	public:
		static void init(EngineInfo engine_info);
		static void deinit();

		static void terminate();
		static void __error(std::string error_msg, int line, std::string file);

		static graphics::Instance* createInstance(const InstanceInfo& instance_config);
		static void destroyInstance(graphics::Instance* instance);
	
};

}