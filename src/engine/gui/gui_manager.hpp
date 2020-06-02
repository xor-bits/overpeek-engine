#pragma once

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"



namespace oe::gui {

	class Widget;
	class Form;
	class GUI {
	private:
		oe::gui::Form* m_main_frame;
		oe::graphics::Renderer* m_renderer;
		oe::graphics::Renderer* m_late_renderer;
		oe::graphics::Window* m_window;
		oe::assets::DefaultShader* m_shader;

		glm::vec2 m_offset;
		glm::vec2 m_old_window_size;

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
		void render_empty();

		// move the whole gui system
		void offset(const glm::vec2& offset);

		oe::graphics::Renderer* getRenderer()
		{
			return m_renderer;
		}

		oe::graphics::Renderer* getLateRenderer()
		{
			return m_late_renderer;
		}

		oe::graphics::Window* getWindow()
		{
			return m_window;
		}
		
	};

}