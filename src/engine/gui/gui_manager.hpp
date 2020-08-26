#pragma once

#include "engine/interfacegen.hpp"
#include "engine/engine.hpp"
#include <entt/entt.hpp>



/* -- forward declarations -- */
namespace oe::assets { class DefaultShader; }
namespace oe::graphics { class kokokookoo; class Font; class Renderer; }
namespace oe::gui { class Widget; class Form; class GUI; }
/* -- forward declarations -- */



namespace oe::gui
{
	constexpr int border = 5;

	struct GUIRenderEvent
	{};
	
	class GUI {
	public:
		entt::dispatcher dispatcher;

	private:
		Form* m_main_frame;
		oe::graphics::Renderer* m_renderer;
		oe::graphics::Renderer* m_line_renderer;
		oe::graphics::Renderer* m_late_renderer;
		oe::assets::DefaultShader* m_shader_fill;
		oe::assets::DefaultShader* m_shader_lines;
		oe::ResizeEvent latest_resize_event;
		const oe::graphics::Window& m_window;

		std::unordered_map<size_t, std::unordered_map<std::string, oe::graphics::Font*>> m_fontmap; // fontsize - ( fontname - font pair) pair
		const std::string m_default_font_path;

		glm::ivec2 m_offset;
		glm::ivec2 m_old_window_size;

	public:
		GUI(const oe::graphics::Window& window, const std::string& default_font = oe::default_full_font_path);
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

		oe::graphics::Renderer* getRenderer() const;
		oe::graphics::Renderer* getLineRenderer() const;
		oe::graphics::Renderer* getLateRenderer() const;
		const oe::graphics::Window& getWindow() const;

		oe::graphics::Font& getFont(size_t resolution, std::string font = "");
	};

}