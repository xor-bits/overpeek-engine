#pragma once

// Graphics
#include "graphics/gl.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/gltexture.h"
#include "graphics/framebuffer.h"
#include "graphics/text/font.h"
#include "graphics/text/textLabel.h"

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
#include "graphics/assets/multiTextureShader.h"
#include "graphics/assets/singleTextureShader.h"
#include "graphics/assets/basicPostprocessShader.h"
#include "networking/client.h"
#include "networking/server.h"



namespace oe {

	class Engine {
	public:
		static EngineInfo m_engine_info;

	public:
		static void init(EngineInfo engine_info);
		static void deinit();

		static void terminate();
		static void __error(std::string error_msg, int line, std::string file);
	};

}