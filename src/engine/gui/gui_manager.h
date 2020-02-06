#pragma once

#include "widgets/widget.h"
#include "widgets/form.h"
#include "widgets/text_panel.h"
#include "widgets/text_input.h"
#include "widgets/button.h"
#include "widgets/button_decorated.h"
#include "widgets/slider.h"
#include "widgets/sprite_panel.h"

#include "engine/graphics/interface/window.h"
#include "engine/graphics/interface/renderer.h"
#include "engine/graphics/interface/shader.h"

#include "engine/graphics/assets/default_shader.hpp"



namespace oe::gui {

	class GUI {
	private:
		std::unique_ptr<Form> m_main_frame;
		oe::graphics::Renderer* m_renderer;
		oe::graphics::Renderer* m_font_renderer;
		oe::graphics::DefaultShader* m_shader;
		oe::graphics::Window* m_window;
		oe::graphics::Instance* m_instance;

	public:
		GUI(oe::graphics::Instance* instance, oe::graphics::Window* window);
		~GUI();
		
		// needs to be after every window resize
		void resize(const glm::vec2& window_size);

		// calls resize(window width, window height)
		void resize();

		void cursor(oe::mouse_buttons button, oe::actions action, const glm::vec2& cursor_window);
		void text(uint32_t codepoint, oe::modifiers mods);
		void key(oe::keys key, oe::actions action, oe::modifiers mods);
		
		// assigns std::unique_ptr for this pointer
		// do forget pointer to this widget
		void addSubWidget(Widget* widget);
		
		// bind SpritePacker that you used to create Font and all Sprites for StaticTextureViews
		void render();
		
	};

}