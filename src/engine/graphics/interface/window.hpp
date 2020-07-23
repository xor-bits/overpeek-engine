#pragma once

#include <string>

#include "engine/graphics/interface/instance.hpp"
#include "engine/graphics/interface/renderer.hpp"
#include "engine/graphics/interface/shader.hpp"
#include "engine/graphics/interface/texture.hpp"
#include "engine/graphics/interface/framebuffer.hpp"
#include "engine/internal_libs.hpp"
#include "engine/enum.hpp"
#include "engine/utility/fileio.hpp"
#include "engine/utility/gameloop.hpp"


#define M_NUM_KEYS		2048
#define M_NUM_BUTTONS	1024



class GLFWwindow;
namespace oe::graphics {

	class IWindow {
	public:
		WindowInfo m_window_info;
		entt::dispatcher dispatcher{};

	protected:
		GLFWwindow* m_window_handle = nullptr;

		bool m_keys[M_NUM_KEYS];
		bool m_buttons[M_NUM_BUTTONS];

		glm::vec2 m_cursor_transformed = { 0.0f, 0.0f };
		glm::ivec2 m_cursor_window = { 0, 0 };
		float m_aspect_ratio = 0;

		oe::utils::GameLoop m_window_gameloop;

		void postglfw();

	public:
		IWindow(const Instance* instance, const WindowInfo& window_config);
		virtual ~IWindow();

		virtual void update() = 0;
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;
		virtual void viewport() = 0;
		virtual void bind() = 0;

		/*Also known as VSync*/
		virtual void swapInterval(uint8_t frames) = 0;

		bool shouldClose();
		void setIcon(const oe::utils::image_data& image);
		void showCursor(bool show);

		// for multiwindow setups
		// active window while rendering needs to be specified first
		virtual void active_context() const = 0;

	public:
		inline const WindowInfo& getWindowInfo() { return m_window_info; }
		inline oe::utils::GameLoop& getGameloop() {return m_window_gameloop; }

		float aspect();
		float button(oe::mouse_buttons button);
		float key(oe::keys key);

		const glm::ivec2& getPosition();
		void setPosition(const glm::ivec2& pos);

		const glm::ivec2& getSize();
		void setSize(const glm::ivec2& size);

		const std::string& getTitle();
		void setTitle(const std::string& title);

		bool getBorderless();
		void setBorderless(bool borderless);

		bool getResizeable();
		void setResizeable(bool resizeable);

		bool getFullscreen();
		void setFullscreen(bool fullscreen);

		const glm::ivec2& getCursorWindow();
		void setCursorWindow(const glm::ivec2& cursor_at_window);

		const glm::vec2& getCursorTransformed();
		void setCursorTransformed(const glm::vec2& cursor_at_world_space);

		const std::string getClipboard();
		void setClipboard(const std::string& str);

		// -- events --
		// connect events
		template<typename Event, auto Listener, typename Instance>
		void connect_listener(const Instance& instance)
		{
			dispatcher.sink<Event>().template connect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			dispatcher.sink<Event>().template connect<Listener>();
		}
		// disconenct events
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(Instance&& instance)
		{
			dispatcher.sink<Event>().template disconnect<Listener>(instance);
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher.sink<Event>().template disconnect<Listener>();
		}
		// connect update
		template<size_t ups, auto F, typename Instance>
		void connect_update_listener(Instance&& instance)
		{
			m_window_gameloop.connect_update_listener<ups, F>(instance);
		}
		template<size_t ups, auto F>
		void connect_update_listener()
		{
			m_window_gameloop.connect_update_listener<ups, F>();
		}
		// disconnect update
		template<size_t ups, auto F, typename Instance>
		void disconnect_update_listener(Instance&& instance)
		{
			m_window_gameloop.disconnect_update_listener<ups, F>(instance);
		}
		template<size_t ups, auto F>
		void disconnect_update_listener()
		{
			m_window_gameloop.disconnect_update_listener<ups, F>();
		}
		// connect render
		template<auto F, typename Instance>
		void connect_render_listener(Instance&& instance)
		{
			m_window_gameloop.connect_render_listener<F>(instance);
		}
		template<auto F>
		void connect_render_listener()
		{
			m_window_gameloop.connect_render_listener<F>();
		}
		// disconnect render
		template<auto F, typename Instance>
		void disconnect_render_listener(Instance&& instance)
		{
			m_window_gameloop.disconnect_render_listener<F>(instance)
		}
		template<auto F>
		void disconnect_render_listener()
		{
			m_window_gameloop.disconnect_render_listener<F>()
		}



		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;
	};

}