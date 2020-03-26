#pragma once

#include "widgets/widget.hpp"
#include "widgets/form.hpp"
#include "widgets/text_panel.hpp"
#include "widgets/text_input.hpp"
#include "widgets/button.hpp"
#include "widgets/button_decorated.hpp"
#include "widgets/slider.hpp"
#include "widgets/sprite_panel.hpp"
#include "widgets/checkbox.hpp"

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"



namespace oe::gui {

	class GUI {
	private:
		oe::graphics::Renderer* m_renderer;
		oe::graphics::Shader* m_shader;
		oe::graphics::Window* m_window;
		Form* m_main_frame;

		glm::vec2 m_offset;

	public:
		GUI(oe::graphics::Window* window);
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

		// move the whole gui system
		void offset(const glm::vec2& offset);
		
	};

}