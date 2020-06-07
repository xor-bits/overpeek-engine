#pragma once

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"



namespace oe::gui {

	struct GUIRenderEvent
	{
		float *z = nullptr;
		oe::graphics::Renderer* renderer = nullptr;
	};

	class Widget;
	class Form;
	class GUI {
	public:
		entt::dispatcher dispatcher{};

	private:
		oe::gui::Form* m_main_frame;
		oe::graphics::Renderer* m_renderer;
		oe::graphics::Renderer* m_late_renderer;
		oe::graphics::Window* m_window;
		oe::assets::DefaultShader* m_shader;

		glm::ivec2 m_offset;
		glm::ivec2 m_old_window_size;

	public:
		GUI(oe::graphics::Window* window);
		~GUI();

		void short_resize();
		
		// events
		void on_resize(const oe::ResizeEvent& event);
		
		// this class will take ownership of this pointer
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