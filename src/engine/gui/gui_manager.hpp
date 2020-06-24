#pragma once

#include "engine/graphics/interface/window.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/assets/default_shader/default_shader.hpp"
#include "engine/interfacegen.hpp"
#include "widgets/widget.hpp"



namespace oe::gui {

	constexpr int border = 5;

	class GUI;
	struct GUIBase
	{
		std::function<const glm::vec2&()> get_size_func;
		std::function<void(GUI *const &)> resize_connect_func;
		std::function<void(GUI *const &)> resize_disconnect_func;
		oe::graphics::Window event_window;

		GUIBase(const std::function<const glm::vec2&()>& _get_size_func, const std::function<void(GUI *const &)>& _resize_connect_func, const std::function<void(GUI *const &)>& _resize_disconnect_func, const oe::graphics::Window& _event_window)
			: get_size_func(_get_size_func)
			, resize_connect_func(_resize_connect_func)
			, resize_disconnect_func(_resize_disconnect_func)
			, event_window(_event_window)
		{}
	};

	struct GUIRenderEvent
	{
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
		GUIBase m_guibase;
		oe::assets::DefaultShader m_shader;

		glm::ivec2 m_offset;
		glm::ivec2 m_old_window_size;

	public:
		GUI(const GUIBase& base);
		GUI(const oe::graphics::Window& window);
		~GUI();

		void short_resize();
		
		// events
		void on_resize(const oe::ResizeEvent& event);
		
		// this class will take ownership of this pointer
		void addSubWidget(Widget* widget);
		void removeSubWidget(Widget* widget);
		
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

		oe::graphics::Window& getWindow()
		{
			return m_guibase.event_window;
		}
	};

}