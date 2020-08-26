#pragma once

#include <string>
#include <mutex>

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
		entt::dispatcher dispatcher;
		std::mutex dispatcher_mutex;

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
		IWindow(const std::unique_ptr<Instance>& instance, const WindowInfo& window_config);
		virtual ~IWindow();

		virtual void update() = 0;
		virtual void clear(const glm::vec4& color = oe::colors::clear_color) = 0;
		virtual void viewport() const = 0;
		virtual void bind() const = 0;

		virtual void pollEvents() = 0;
		virtual void waitEvents(float timeout = 0.0f) = 0; // timeout in seconds, 0 for no timeout
		virtual void updateEvents() = 0; // only after for waitEvents, process events in main thread
		virtual void bump() = 0; // empty event to get out of waitEvents

		/*Also known as VSync*/
		virtual void swapInterval(uint8_t frames) = 0;

		bool shouldClose();
		void setIcon(const oe::utils::image_data& image);
		void showCursor(bool show);

		// for multiwindow setups
		// active window while rendering needs to be specified first
		virtual void active_context() const = 0;
		virtual void inactive_context() const = 0;

	public:
		inline const WindowInfo& getWindowInfo() { return m_window_info; }
		inline oe::utils::GameLoop& getGameloop() {return m_window_gameloop; }

		float aspect();
		bool button(oe::mouse_buttons button);
		bool key(oe::keys key);

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
			dispatcher_mutex.lock();
			dispatcher.sink<Event>().template connect<Listener>(instance);
			dispatcher_mutex.unlock();
		}
		template<typename Event, auto Listener>
		void connect_listener()
		{
			dispatcher_mutex.lock();
			dispatcher.sink<Event>().template connect<Listener>();
			dispatcher_mutex.unlock();
		}
		// disconenct events
		template<typename Event, auto Listener, typename Instance>
		void disconnect_listener(const Instance& instance)
		{
			dispatcher_mutex.lock();
			dispatcher.sink<Event>().template disconnect<Listener>(instance);
			dispatcher_mutex.unlock();
		}
		template<typename Event, auto Listener>
		void disconnect_listener()
		{
			dispatcher_mutex.lock();
			dispatcher.sink<Event>().template disconnect<Listener>();
			dispatcher_mutex.unlock();
		}
		// connect update
		template<size_t ups, auto F, typename Instance>
		void connect_update_listener(const Instance& instance)
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
		void disconnect_update_listener(const Instance& instance)
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
		void connect_render_listener(const Instance& instance)
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
		void disconnect_render_listener(const Instance& instance)
		{
			m_window_gameloop.disconnect_render_listener<F>(instance);
		}
		template<auto F>
		void disconnect_render_listener()
		{
			m_window_gameloop.disconnect_render_listener<F>();
		}
		// connect init
		template<auto F, typename Instance>
		void connect_init_listener(const Instance& instance)
		{
			m_window_gameloop.connect_init_listener<F>(instance);
		}
		template<auto F>
		void connect_init_listener()
		{
			m_window_gameloop.connect_init_listener<F>();
		}
		// disconnect init
		template<auto F, typename Instance>
		void disconnect_init_listener(const Instance& instance)
		{
			m_window_gameloop.disconnect_init_listener<F>(instance);
		}
		template<auto F>
		void disconnect_init_listener()
		{
			m_window_gameloop.disconnect_init_listener<F>();
		}
		// connect cleanup
		template<auto F, typename Instance>
		void connect_cleanup_listener(const Instance& instance)
		{
			m_window_gameloop.connect_cleanup_listener<F>(instance);
		}
		template<auto F>
		void connect_cleanup_listener()
		{
			m_window_gameloop.connect_cleanup_listener<F>();
		}
		// disconnect cleanup
		template<auto F, typename Instance>
		void disconnect_cleanup_listener(const Instance& instance)
		{
			m_window_gameloop.disconnect_cleanup_listener<F>(instance);
		}
		template<auto F>
		void disconnect_cleanup_listener()
		{
			m_window_gameloop.disconnect_cleanup_listener<F>();
		}



		virtual std::string getAPI() const = 0;
		virtual std::string getAPIVersion() const = 0;
		virtual std::string getGPU() const = 0;
		virtual std::string getGPUVendor() const = 0;
	};

}