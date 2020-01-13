#pragma once

#include "widgets/widget.h"

#include <engine/graphics/renderer.h>
#include <engine/graphics/assets/multiTextureShader.h>



namespace oe::gui {

	class GUI {
	private:
		static Widget* m_main_frame;
		static oe::graphics::Renderer* m_renderer;
		static oe::graphics::MultiTextureShader* m_shader;

		static bool m_initialized;

	public:
		static void init();
		static void resize(int width, int height);
		static void resize();
		static void render();
		
	};

}