#pragma once

// Graphics
#include "graphics/interface/window.h"
#include "graphics/interface/shader.h"
#include "graphics/interface/renderer.h"
#include "graphics/interface/texture.h"
#include "graphics/interface/framebuffer.h"
#include "graphics/interface/instance.h"
#include "graphics/font.h"
#include "graphics/textLabel.h"

// Audio
#include "audio/audio.h"

// Logic
#include "utility/zip.h"
#include "utility/math.h"
#include "utility/clock.h"
#include "utility/debug.h"
#include "utility/random.h"
#include "utility/gameloop.h"
#include "utility/filereader.h"

// Gui
#include "gui/gui_manager.h"

// internal libs
#include "internal_libs.h"

// Other
#include "graphics/sprite.h"
#include "graphics/spritePacker.h"
#include "networking/client.h"
#include "networking/server.h"
#include "graphics/assets/default_shader.hpp"



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