#pragma once

// Graphics
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/texture.h"
#include "graphics/framebuffer.h"
#include "graphics/text/font.h"
#include "graphics/text/textLabel.h"

// opengl abstraction
#include "graphics/opengl/gl_window.h"
#include "graphics/opengl/gl_renderer.h"
#include "graphics/opengl/gl_shader.h"

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
		static EngineInfo engine_info;

	public:
		static void init(EngineInfo engine_info);
		static void deinit();

		static void terminate();
		static void __error(std::string error_msg, int line, std::string file);

		static void destroyWindow(graphics::Window* window) {
			delete window;
		}

		static void destroyRenderer(graphics::Renderer* renderer) {
			delete renderer;
		}

		static void destroyShader(graphics::Shader* shader) {
			delete shader;
		}

		static graphics::Window* createWindow(WindowInfo& window_config) {
			if (engine_info.api == oe::Vulkan) {
				// Vulkan window
				
			} else {
				return new oe::graphics::GLWindow(window_config);
			}
		}

		static graphics::Renderer* createRenderer(const RendererInfo& renderer_info) {
			if (engine_info.api == oe::Vulkan) {
				// Vulkan renderer

			}
			else {
				return new oe::graphics::GLRenderer(renderer_info);
			}
		}

		static graphics::Shader* createShader(const ShaderInfo& shader_info) {
			if (engine_info.api == oe::Vulkan) {
				// Vulkan shader

			}
			else {
				return new oe::graphics::GLShader(shader_info);
			}
		}
	};

}