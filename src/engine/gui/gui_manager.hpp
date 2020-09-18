#pragma once

#include "engine/interfacegen.hpp"
#include "engine/engine.hpp"
#include "widgets/form.hpp"
#include <entt/entt.hpp>



/* -- forward declarations -- */
namespace oe::assets { class DefaultShader; }
namespace oe::graphics { class kokokookoo; class Font; class Renderer; }
namespace oe::gui { class Widget; class Form; class GUI; }
/* -- forward declarations -- */



namespace oe::gui
{
	constexpr int border = 5;

	struct GUIRenderEvent {};
	struct GUIPreRenderEvent {};
	
	class GUI {
	public:
		entt::dispatcher dispatcher;

	private:
		std::shared_ptr<Form> m_main_frame;
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
		glm::uvec2 m_old_window_size;

	public:
		GUI(const oe::graphics::Window& window, const std::string& default_font = oe::default_full_font_path, int32_t renderer_primitive_count = 100000);
		~GUI();

		void short_resize();
		
		// events
		void on_resize(const oe::ResizeEvent& event);

		template<typename T, typename ... Args>
		std::shared_ptr<T> create(const Args& ... args)
		{
			return m_main_frame->create<T>(args...);
		}
		template<typename T>
		void remove(const std::shared_ptr<T>& widget)
		{
			return m_main_frame->remove<T>(widget);
		}
		
		// bind SpritePacker that you used to create Font and all Sprites for StaticTextureViews
		void render();
		void render_empty();

		// move the whole gui system
		void offset(const glm::vec2& offset);

		inline oe::graphics::Renderer* getRenderer() const { return m_renderer; }
		inline oe::graphics::Renderer* getLineRenderer() const { return m_line_renderer; }
		inline oe::graphics::Renderer* getLateRenderer() const { return m_late_renderer; }
		inline const oe::graphics::Window& getWindow() const { return m_window; }
		inline const oe::assets::DefaultShader* getShaderFill() const { return m_shader_fill; }
		inline const oe::assets::DefaultShader* getShaderLines() const { return m_shader_lines; }

		oe::graphics::Font& getFont(uint16_t resolution, const std::string& font = "");
	};

}