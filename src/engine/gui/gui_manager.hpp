#pragma once

#include "engine/interfacegen.hpp"
#include "engine/engine.hpp"
#include "engine/utility/fileio.hpp"
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
		oe::assets::DefaultShader* m_shader_fill;
		oe::assets::DefaultShader* m_shader_lines;
		oe::ResizeEvent latest_resize_event;
		oe::graphics::Window m_window;

		std::unordered_map<size_t, std::unordered_map<oe::utils::FontFile, oe::graphics::Font>> m_fontmap; // fontsize - ( fontfile - font pair) pair
		const oe::utils::FontFile m_default_font_file;

		glm::vec2 m_old_render_size;

		glm::vec2 m_offset;
		glm::vec2 m_size_mult;
		glm::mat4 m_render_ml_matrix;
		glm::mat4 m_cursor_ml_matrix;

		void updateModelMatrix();

	public:
		GUI(const oe::graphics::Window& window, const oe::utils::FontFile& font_file = {}, int32_t renderer_primitive_count = 100000);
		~GUI();

		void short_resize();

		template<typename T, typename ... Args>
		std::shared_ptr<T> create(Args& ... args)
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

		// used for application debugging
		void zoom(const glm::vec2& mult);

		inline oe::graphics::Renderer* getRenderer() const { return m_renderer; }
		inline oe::graphics::Renderer* getLineRenderer() const { return m_line_renderer; }
		inline const oe::graphics::Window& getWindow() const { return m_window; }
		inline const oe::assets::DefaultShader* getShaderFill() const { return m_shader_fill; }
		inline const oe::assets::DefaultShader* getShaderLines() const { return m_shader_lines; }

		oe::graphics::Font& getFont(uint16_t resolution, const oe::utils::FontFile& font = {});

	private:
		// events
		void on_resize(const ResizeEvent& event);
		void on_codepoint(const CodepointEvent& event);
		void on_key(const KeyboardEvent& event);
		void on_cursor_pos(const CursorPosEvent& event);
		void on_button(const MouseButtonEvent& event);
		void on_scroll(const ScrollEvent& event);
		oe::utils::connect_guard m_cg_resize;
		oe::utils::connect_guard m_cg_codepoint;
		oe::utils::connect_guard m_cg_key;
		oe::utils::connect_guard m_cg_cursor_pos;
		oe::utils::connect_guard m_cg_button;
		oe::utils::connect_guard m_cg_scroll;
	};

}